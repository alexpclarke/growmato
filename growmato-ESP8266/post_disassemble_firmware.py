Import("env")

env.AddPostAction(
  "$BUILD_DIR/firmware.elf",
  env.VerboseAction("xtensa-lx106-elf-objdump -d $BUILD_DIR/firmware.elf > $BUILD_DIR/firmware.lss", "Creating $BUILD_DIR/firmware.lss")
)