BUILD_DIR		= cmake-build-release
INSTALL_DIR		= /usr/bin

.PHONY: all install clean

all: $(BUILD_DIR)
	$(MAKE) -C $<

$(BUILD_DIR): CMakeLists.txt
	mkdir -p $@
	cd $@ && cmake -D CMAKE_BUILD_TYPE=Release ..

install: eupdatedb
	$(MAKE) -C $(BUILD_DIR) install
	cp $^ $(DESTDIR)$(INSTALL_DIR)/

clean:
	rm -rf $(BUILD_DIR)
