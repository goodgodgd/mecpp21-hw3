sudo apt install libopencv-dev

        PinholeCameraIntrinsicParameters param) {
    if (param == PinholeCameraIntrinsicParameters::PrimeSenseDefault)
        SetIntrinsics(640, 480, 525.0, 525.0, 319.5, 239.5);
    else if (param ==
             PinholeCameraIntrinsicParameters::Kinect2DepthCameraDefault)
        SetIntrinsics(512, 424, 365.456, 365.456, 254.878, 205.395);
    else if (param ==
             PinholeCameraIntrinsicParameters::Kinect2ColorCameraDefault)
        SetIntrinsics(1920, 1080, 1059.9718, 1059.9718, 975.7193, 545.9533);

https://modoocode.com/304



