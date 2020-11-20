

// 欧拉角转换为旋转向量
Eigen::AngleAxisd rotation_vector;
rotation_vector = Eigen::AngleAxisd(poseArray[3], Eigen::Vector3d::UnitZ())                                            * 
Eigen::AngleAxisd(poseArray[4], Eigen::Vector3d::UnitY()) * 
Eigen::AngleAxisd(poseArray[5], Eigen::Vector3d::UnitX());  
Eigen::Vector3d rot_vect = rotation_vector.axis().transpose() * rotation_vector.angle();
poseArray[3] = rot_vect[0];
poseArray[4] = rot_vect[1];
poseArray[5] = rot_vect[2];


// 旋转向量转换为欧拉角
Eigen::Vector3d theta_rad_v = Eigen::Vector3d(poseArray[3], poseArray[4], poseArray[5]);
Eigen::AngleAxisd rotation_vector (theta_rad_v.norm(), theta_rad_v.normalized()); 
Eigen::Vector3d eulerAngle = rotation_vector.matrix().eulerAngles(2,1,0);
poseArray[3] = eulerAngle[0];
poseArray[4] = eulerAngle[1];
poseArray[5] = eulerAngle[2];
