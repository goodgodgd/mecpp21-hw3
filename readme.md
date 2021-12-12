# 인공지능특론 Homework #3



## 최적의 point cloud filter 구현

본 숙제의 내용은 point cloud를 위한 필터를 구현하는 것이다.  

어떤 point cloud를 불러와서 노이즈를 추가한 뒤 필터를 적용하여 노이즈를 최대한 제거한 point cloud를 복원하는 것이 목표다.  

기본적인 프로젝트의 틀은 [mecpp21-hw3](https://github.com/goodgodgd/mecpp21-hw3)에서 받을 수 있으며 여기에 필터 구현만 추가하면 된다.  

구체적으로 말하면 filter.h/cpp 파일에서 `FilterBase`에서 파생된 필터 클래스를 주어진 조건에 따라 구현하면 된다.  



### 1. main 함수 구조

```cpp
void PCPMain::main()
{
    // 1. colored point cloud를 불러온다.
    cv::Mat color = load_image(color_name, cv::IMREAD_COLOR);
    cv::Mat depth = load_image(depth_name, cv::IMREAD_ANYDEPTH);
    cv::Mat cloud = convert_to_point_cloud(depth);
    // 원본 point cloud를 보여준다.
    show_point_cloud(cloud, color);
    
    // 2. depth noise를 추가한다.
    cv::Mat noisy_cloud = add_noise_cloud(cloud);
    // noise가 추가된 point cloud를 보여준다.
    show_point_cloud(noisy_cloud, color);
    // 3. noise가 추가된 point cloud에 다양한 필터를 적용하고 그 결과를 확인한다.
    for (auto &filter : filters)
    {
        // 필터 적용 및 속도 측정
        cv::Mat smooth_cloud = smooth_filter(filter, noisy_cloud);
        // 필터링 성능 측정
        evaluate_filter(cloud, smooth_cloud);
    }
}
```



### 2. Filter 조건

- `FilterBase` 클래스를 상속해야 한다.
- 5x5 사이즈의 커널을 적용한다.
- depth가 유사한 사물 내부의 점들에 대해서는 smoothing을 한다.
- depth가 급격히 달라지는 사물 외곽에서는 smoothing을 제한한다.
- 필터 내부에서는 cv::Mat이나 cv::Vec3f를 쓰지 않고 자체 구현한 데이터 형식 사용
  - e.g. HW1, 2에서 만든 Matrix 클래스
- 하나의 클래스만 구현 하는 것이 아니라 최적화 수준에 따라 다양한 필터 구현
  - 연산 과정은 같으나 최적화 수준에 따라 속도 달라지는 것 확인
  - 전혀 최적화를 하지 않은 필터부터 가장 최적화를 적용한 필터까지 **최소 3개** 구현
  - 필요하다면 Matrix 클래스를 최적화 수준에 따라 따로 구현해도 됨
  - 불필요한 연산이 들어가서는 안됨



### 3. 평가 포인트

1. 최적화를 적용한 것과 하지 않은 것의 처리 시간 차이, 최소 처리 시간
2. 필터링의 노이즈 제거 수준
   1. 원본과 비교한 평균 오차
   2. 원본과 비교한 최대 오차
3. 코드의 질적 수준
   1. 가독성
   2. 강건성 (잘못 사용되기 어려운 코드)
   3. 간결성 (중복 제거, 최소의 구현)



### 4. 제출

- 12월 20일 월요일까지
- 원본 깃헙에 Pull Request를 올리기
- PR 내용에 자신이 측정한 평가 포인트 1, 2 결과를 보고서처럼 정리



## 참고

OpenCV 간편 설치

```
sudo apt install libopencv-dev
```

혹은 직접 빌드해도 됩니다. 빌드하는 방법은 여러 블로그에 올라와 있으니 검색해보시길...

