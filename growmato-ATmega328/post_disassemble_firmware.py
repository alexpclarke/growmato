Import("env")

env.AddPostAction(
  "$BUILD_DIR/firmware.elf",
  env.VerboseAction("avr-objdump -d $BUILD_DIR/firmware.elf > $BUILD_DIR/firmware.lss", "Creating $BUILD_DIR/firmware.lss")
)