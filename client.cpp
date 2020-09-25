#include <iostream>

#include <cstdio>   // perror()
#include <cstring>  // memset()
#include <unistd.h> // close()

#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>

#define AXIS_COUNT 8
#define ANALOG_IN_NUM 3
#define ANALOG_OUT_NUM 5

static int PORT = 8056;

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
    server_addr.sin_addr.s_addr = inet_addr("127.0.0.1");
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
    printf("recv byte = %d\n", ret);
    printf("messageSize = %d\n", rcv_buf.messageSize);
    printf("timestamp = %lu\n", rcv_buf.timestamp);
    printf("autorunCycelMode = %d\n", rcv_buf.autorunCycelMode);
    printf("robotState = %d\n", rcv_buf.robotState);
    printf("servoReady = %d\n", rcv_buf.servoReady);
    printf("canMotorRun = %d\n", rcv_buf.canMotorRun);
    printf("motorSpeed[0] = %d\n", rcv_buf.motorSpeed[0]);
    printf("robotMode = %d\n", rcv_buf.robotMode);

    //6. 关闭服务套接口close
    close(socket_fd);

    return 0;
}
