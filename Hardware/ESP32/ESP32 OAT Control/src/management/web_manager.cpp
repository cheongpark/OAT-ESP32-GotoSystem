#include "management/web_manager.hpp"
#include "log.hpp"

namespace WEB {
    WebManager* WebManager::instance = nullptr;
    
    WebManager::WebManager() {
        server = new AsyncWebServer(80);
    }
    
    WebManager& WebManager::getInstance() {
        static WebManager instance;
        return instance;
    }
    
    void WebManager::init() {
        // 루트 경로 처리
        server->on("/", HTTP_GET, [](AsyncWebServerRequest *request) {
            String html = String(index_html);
            html.replace("%STYLE_PLACEHOLDER%", style_css);
            html.replace("%SCRIPT_PLACEHOLDER%", script_js);
            request->send(200, "text/html", html, [](const String& var) {
                return WebManager::getInstance().processWebVariables(var);
            });
        });
        
        // RA/DEC 컨트롤 엔드포인트
        server->on("/ra-dec/control", HTTP_GET, [](AsyncWebServerRequest *request) {
            WebManager::getInstance().handleRaDecControl(request);
        });
        server->on("/ra-dec/stop", HTTP_GET, [](AsyncWebServerRequest *request) {
            WebManager::getInstance().handleRaDecStop(request);
        });
        
        // AZ/ALT 컨트롤 엔드포인트
        server->on("/az-alt/control", HTTP_GET, [](AsyncWebServerRequest *request) {
            WebManager::getInstance().handleAzAltControl(request);
        });
        
        // 긴급 정지 엔드포인트
        server->on("/stop", HTTP_GET, [](AsyncWebServerRequest *request) {
            WebManager::getInstance().handleStop(request);
        });
        
        server->on("/sensordata", HTTP_GET, [](AsyncWebServerRequest *request) {
            // 센서 데이터 처리
        });
        
        server->on("/systeminfo", HTTP_GET, [](AsyncWebServerRequest *request) {
            // 시스템 정보 처리
        });
        
        server->on("/led/toggle", HTTP_GET, [](AsyncWebServerRequest *request) {
            request->send(200, "application/json");
        });
        
        server->on("/led", HTTP_GET, [](AsyncWebServerRequest *request) {
            // LED 상태 처리
        });
        
        // 서버 시작
        server->begin();
        LOG(LOG_INFO, "웹 서버 시작됨");
    }
    
    String WebManager::processWebVariables(const String& var) {
        if(var == "GPSINFO")
            return F("asdsdasadasdasd");
        if(var == "FIRMWAREMODEL")
            return F(Mount::Data::getInstance().get_info_product_name().c_str());
        if(var == "FIRMWAREVERSION")
            return F(Mount::Data::getInstance().get_info_firmware_version().c_str());
        return String();
    }

    void WebManager::handleRaDecControl(AsyncWebServerRequest* request) {
        if (request->hasParam("direction")) {
            char direction = request->getParam("direction")->value()[0];

            if(direction == 'n' || direction == 's' || direction == 'e' || direction == 'w') {
                LOG(LOG_INFO, "RA/DEC Control: " + direction);
                control.startRaDecSlewing(direction);
                request->send(200, "application/json", "{\"status\":\"ok\"}");
            }
            else {
                LOG(LOG_ERROR, "RA/DEC Control: " + direction);
                request->send(400, "application/json", "{\"status\":\"error\"}");
            }
        }
    }

    void WebManager::handleRaDecStop(AsyncWebServerRequest* request) {
        if (request->hasParam("direction")) {
            char direction = request->getParam("direction")->value()[0];

            if(direction == 'n' || direction == 's' || direction == 'e' || direction == 'w') {
                LOG(LOG_INFO, "RA/DEC Stop: " + direction);
                control.stopRaDecSlewing(direction);
                request->send(200, "application/json", "{\"status\":\"ok\"}");
            }
            else {
                LOG(LOG_ERROR, "RA/DEC Stop: " + direction);
                request->send(400, "application/json", "{\"status\":\"error\"}");
            }
        }
    }

    void WebManager::handleAzAltControl(AsyncWebServerRequest* request) {
        if (request->hasParam("direction")) {
            char direction = request->getParam("direction")->value()[0];

            if(direction == 'n' || direction == 's' || direction == 'e' || direction == 'w') {
                LOG(LOG_INFO, "AZ/ALT Control: " + String(direction));
                control.startAzAltSlewing(direction);
                request->send(200, "application/json", "{\"status\":\"ok\"}");
            }
            else {
                LOG(LOG_ERROR, "AZ/ALT Control: " + direction);
                request->send(400, "application/json", "{\"status\":\"error\"}");
            }
        }
    }

    void WebManager::handleStop(AsyncWebServerRequest* request) {
        LOG(LOG_INFO, "긴급 정지 요청");
        
        // 모든 모터 정지
        control.stopSlewing();
        
        request->send(200, "application/json", "{\"status\":\"ok\"}");
    }

    void WebManager::handleAutoHome(AsyncWebServerRequest* request) {
        // 자동 홈 처리
    }

    void WebManager::handleHome(AsyncWebServerRequest* request) {
        // 홈 처리
    }

    void WebManager::updateTargetRA(int h, int m, int s) {
        // RA 타겟 업데이트
    }

    void WebManager::updateTargetDEC(int d, int m, int s) {
        // DEC 타겟 업데이트
    }

    String WebManager::getCurrentRA() {
        // 현재 RA 값 반환
        return String();
    }

    String WebManager::getCurrentDEC() {
        // 현재 DEC 값 반환
        return String();
    }
}
