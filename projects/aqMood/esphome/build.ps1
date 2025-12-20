echo "Compile C3 variant"
esphome compile .\aq-mood-c3.yaml
echo "Copy C3 variant to bin folder"
cp .\.esphome\build\mista-aq-mood-c3\.pioenvs\mista-aq-mood-c3\firmware.factory.bin .\bin\firmware.c3.bin
echo "Compile C3 Standalone variant"
esphome compile .\aq-mood-c3-standalone.yaml
echo "Copy C3 Standalone variant to bin folder"
cp .\.esphome\build\mista-aq-mood-c3-standalone\.pioenvs\mista-aq-mood-c3-standalone\firmware.factory.bin .\bin\firmware.c3.standalone.bin
echo "Compile S3 variant"
esphome compile .\aq-mood-s3.yaml
echo "Copy S3 variant to bin folder"
cp .\.esphome\build\mista-aq-mood-s3\.pioenvs\mista-aq-mood-s3\firmware.factory.bin .\bin\firmware.s3.bin
echo "Compile S3 Standalone variant"
esphome compile .\aq-mood-s3-standalone.yaml
echo "Copy S3 Standalone variant to bin folder"
cp .\.esphome\build\mista-aq-mood-s3-standalone\.pioenvs\mista-aq-mood-s3-standalone\firmware.factory.bin .\bin\firmware.s3.standalone.bin
