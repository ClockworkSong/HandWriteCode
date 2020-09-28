#include <iostream>

#include <cstdio>   // perror()
#include <cstring>  // memset()
#include <unistd.h> // close()

#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <endian.h>

#include<math.h>

#define AXIS_COUNT 8
#define ANALOG_IN_NUM 3
#define ANALOG_OUT_NUM 5

static int PORT = 8056;

#pragma pack(push)
#pragma pack(1) //C编译器将按照n个字节对齐
typedef struct robot_data {
    uint32_t messageSize;                   // 数据包长度 4Byte
    uint64_t timestamp;                     // 时间戳  8Byte
    uint8_t autorunCycelMode;               // 循环模式 1Byte
    double machinePos[AXIS_COUNT];          // 关节角度，单位：度 64Byte
    double machinePose[6];                  // 直角坐标，X,Y,Z单位：毫米 Rx,Ry,Rz单位：弧度 48Byte
    double machineUserPose[6];              // 用户坐标，X,Y,Z单位：毫米 Rx,Ry,Rz单位：弧度 48Byte
    double torque[AXIS_COUNT];              // 关节额定力矩百分比，单位：百分比 64Byte
    int32_t robotState;                     // 机器人状态，0代表停止，1代表暂停，2代表急停，3代表运行，4代表报警 4Byte
    int32_t servoReady;                     // 抱闸状态，0代表未打开，1代表已打开 4Byte
    int32_t canMotorRun;                    // 同步状态，0代表未同步，1代表同步 4Byte
    int32_t motorSpeed[AXIS_COUNT];         // 电机速度，单位：转/分 32Byte
    int32_t robotMode;                      // 机器人模式，0代表示教模式，1代表自动模式，2代表远程模式 4Byte
    double analogIoInput[ANALOG_IN_NUM];    // 模拟量输入口数据，单位：伏特 24Byte
    double analogIoOutput[ANALOG_OUT_NUM];  // 模拟量输出口数据，单位：伏特 40Byte
    uint64_t digitalIoInput;                // 数字量输入数据  8Byte
    uint64_t digitalIoOutput;               // 数字量输出数据  8Byte
}robot_data_t;
#pragma pack(pop)

//将字节中pos位置开始的len位的二进制数转换为整数
unsigned int getbitu(const unsigned char *buff, int pos, int len)
{
    unsigned int bits=0;
    int i;
    for (i=pos;i<pos+len;i++)
    {
        bits=(bits<<1)+((buff[i/8]>>(7-i%8))&1u);   //从高位到低位逐位计算
    }
    return bits;
}

float HexToFloat(const unsigned char *buf)
{
    float value = 0.0;
    unsigned int i = 0;
    unsigned int num, temp;
    int num2;
    bool flags1 = true;

    num = getbitu(buf, i, 1); //标志位
    i = i + 1;
    //指数部分,float型数据其规定偏移量为127,阶码有正有负，对于8位二进制，则其表示范围为-128-127
    num2 = getbitu(buf, i, 8) - 127;
    i = i + 8;

    while(1)
    {
        if(flags1)
        {
            value += 1 * pow(2, num2);
            num2--;
            flags1 = false;
        }
        temp = getbitu(buf, i, 1);
        i += 1;
        value += temp * pow(2, num2);
        num2--;

        if(i == 32)
            break;
    }

    if(num == 1)
        value *= -1;

    return value;
}

double HexToDouble(const unsigned char* buf)
{
    double value = 0;
    unsigned int i = 0;
    unsigned int num,temp;
    int num2;
    bool flags1 = true;

    num = getbitu(buf,i,1); //标志位               
    i += 1;
    //double型规定偏移量为1023，其表示范围为-1024-1023
    num2 = getbitu(buf,i,11) - 1023;        
    i += 11;    

    while(1)
    {
        if(flags1)
        {
            flags1 = false;
            value += 1 * pow(2,num2); num2--;
        }
        temp = getbitu(buf,i,1);    i += 1;
        value += temp * pow(2,num2); num2--;
        if(i == 64)
            break;
    }
    if(num == 1)
        value *= -1;

    return value;
}

int main()
{
    //1. 调用socket函数创建套接字
    int socket_fd = socket(AF_INET, SOCK_STREAM, 0);// IPv4 Internet protocols
    if( socket_fd == -1) {
        perror("socket error:");
        return -1;
    }

    //2. 向服务器发出连接请求
    struct sockaddr_in server_addr{};
    memset(&server_addr, 0, sizeof(server_addr));
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(PORT);
    server_addr.sin_addr.s_addr = inet_addr("10.211.55.5");
    int socket_connect = connect(socket_fd, (struct sockaddr *) &server_addr, sizeof(sockaddr_in));
    if(socket_connect == -1) {
        perror("bind error:");
        return -1;
    }
    robot_data_t rcv_buf;
    memset(&rcv_buf, 0, sizeof(rcv_buf));
    //5. 调用write/read或send/recv进行数据的读写
    int ret = recv(socket_fd, (char*)&rcv_buf, sizeof(rcv_buf), 0);
    if (ret == -1) {
        perror("recv error:");
    }

    short x = 0x1234;
    printf("before x= %x, after x=%x\n", x, be16toh(x));
    int y = 0x12345678;
    printf("before y= %x, after y=%x\n", y, be32toh(y));
    long z = 0x1234567876543210;
    printf("before z= %lx, after z=%lx\n", z, be64toh(z));

    printf("recv byte = %d\n", ret);
    printf("messageSize = %d\n", be32toh(rcv_buf.messageSize));
    printf("timestamp = %lu\n", be64toh(rcv_buf.timestamp));
    printf("autorunCycelMode = %d\n", rcv_buf.autorunCycelMode);

    long pos0 = rcv_buf.machinePos[0];
    printf("machinePos[0] = %lx\n", be64toh(pos0));
    // printf("machinePos[0] = %lx\n", be64toh(rcv_buf.machinePos[0]));
    // printf("machinePos[1] = %lx\n", be64toh(rcv_buf.machinePos[1]));
    // printf("machinePos[2] = %lx\n", be64toh(rcv_buf.machinePos[2]));
    // printf("machinePos[3] = %lu\n", be64toh(rcv_buf.machinePos[3]));
    // printf("machinePos[4] = %lu\n", be64toh(rcv_buf.machinePos[4]));
    // printf("machinePos[5] = %lu\n", be64toh(rcv_buf.machinePos[5]));

    unsigned char a[] = { 0xe7, 0x6b, 0xf5, 0xec, 0x10, 0x54, 0x5a, 0xc0};
    double *p = (double*)a;
    printf("%f\n", *p);

    printf("robotState = %d\n", be32toh(rcv_buf.robotState));
    printf("servoReady = %d\n", be32toh(rcv_buf.servoReady));
    printf("canMotorRun = %d\n", be32toh(rcv_buf.canMotorRun));
    printf("motorSpeed[0] = %d\n", be32toh(rcv_buf.motorSpeed[0]));
    printf("robotMode = %d\n", be32toh(rcv_buf.robotMode));

    //6. 关闭服务套接口close
    close(socket_fd);

    return 0;
}