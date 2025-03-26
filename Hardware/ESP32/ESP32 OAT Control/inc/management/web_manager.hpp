#pragma once

#include <ESPAsyncWebServer.h>

#include "mount/data.hpp"
#include "page.hpp"
#include "management/control.hpp"

// Web 관련된 것을 전부 관리하는 것
// 싱글톤 패턴으로 구현

namespace WEB {
    class WebManager {
    private:
        AsyncWebServer* server;
        static WebManager* instance;

        Control& control = Control::getInstance();
        
        // 싱글톤 패턴
        WebManager();
        
    public:
        // Singletone
        WebManager(const WebManager&) = delete;
        WebManager& operator=(const WebManager&) = delete;

        static WebManager& getInstance();
        
        void init();
        
        // 웹 페이지 데이터 처리
        String processWebVariables(const String& var);
        
        // RA/DEC 컨트롤
        void handleRaDecControl(AsyncWebServerRequest* request);
        void handleRaDecStop(AsyncWebServerRequest* request);
        
        // AZ/ALT 컨트롤
        // AZ/ALT는 눌렀을 때 한번만 이동하기 때문에 멈추는건 없음, 한번만 움직이고 자동으로 멈춤
        void handleAzAltControl(AsyncWebServerRequest* request);

        // TODO 나머지 핸들 연결하기
        // API 엔드포인트 핸들러들
        void handleStop(AsyncWebServerRequest* request);
        void handleAutoHome(AsyncWebServerRequest* request);
        void handleHome(AsyncWebServerRequest* request);
        
        // RA/DEC 타겟 값 관리
        void updateTargetRA(int h, int m, int s);
        void updateTargetDEC(int d, int m, int s);
        
        // 현재 위치 값 얻기
        String getCurrentRA();
        String getCurrentDEC();
    };
}

