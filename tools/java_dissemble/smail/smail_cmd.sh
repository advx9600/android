java -jar baksmali-2.0.3.jar -a 12 -x Settings.odex  -d .
java -jar smali-2.0.3.jar out/ -o classes.dex
dex2jar.bat classes.dex
