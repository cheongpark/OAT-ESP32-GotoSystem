USER_UID := 1000
USER_GID := 1000

ifeq ($(OS),Windows_NT)
    # Windows
    CURRENT_DIR := $(shell cd)
    SKYDATA_DIR := $(CURRENT_DIR)/../test-skydata
    DOCKER_MOUNTS := -v "$(CURRENT_DIR):/app" -v "$(SKYDATA_DIR):/skydata" $(EXTRA_DOCKER_MOUNTS)
else
    # Linux
    DOCKER_MOUNTS := -v "$(PWD):/app" -v "$(PWD)/../test-skydata:/skydata" $(EXTRA_DOCKER_MOUNTS)
endif

setup: Dockerfile Dockerfile.jsbuild
	@echo "Build docker image for compilation with emscripten"
	docker build -f Dockerfile.jsbuild -t swe-dev .
	@echo "Build docker image for webpack/node development"
	docker build -t stellarium-web-dev --build-arg USER_UID=${USER_UID} --build-arg USER_GID=${USER_GID} .
	docker run -it $(DOCKER_MOUNTS) stellarium-web-dev yarn install

dev:
	docker run -it -p 8080:8080 -p 8888:8888 $(DOCKER_MOUNTS) -e CHOKIDAR_USEPOLLING=true stellarium-web-dev yarn run dev

lint:
	docker run -it -p 8080:8080 -p 8888:8888 $(DOCKER_MOUNTS) stellarium-web-dev yarn run lint

build:
	docker run -it $(DOCKER_MOUNTS) stellarium-web-dev yarn run build

start:
	cd dist && python3 -m http.server 8080

i18n:
	python3 ./tools/update-i18n-en.py
