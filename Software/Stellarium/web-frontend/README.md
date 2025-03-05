도커를 사용하기 때문에 Docker Desktop을 설치 후 실행한 상태로 명령어 사용 가능

Since we use Docker, please install and run Docker Desktop before using the commands

``` bash
# Docker Setup
make setup

# and build and run the web GUI (go to http://localhost:8080 on your machine)
make dev

# Optionally, compile a production version of the site with minification
make build

# and finally to host it on a test server (http://localhost:8000)
make start
```