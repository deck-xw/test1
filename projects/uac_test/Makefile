
PROJECT?=$(notdir $(CURDIR))
ARMINO_DIR1?=/home/deck/bk7256/armino-main/
ARMINO_DIR_TO_TOP?=../../../../../..
PROJECT_DIR?=$(ARMINO_DIR_TO_TOP)$(CURDIR)

$(info PROJECT: $(PROJECT))
$(info PROJECT_DIR: $(PROJECT_DIR))

export CL_SW_VER=$(USER_SW_VER)
export CL_APP_TEMPLATE=$(APP_TEMPLATE)

all:
	mkdir -p build/
	@make -C $(ARMINO_DIR1) bk7256 PROJECT_DIR=$(PROJECT_DIR) PROJECT=app PROJECT_BUILD_DIR=$(PROJECT_DIR)/build/


clean:
	rm -rf build