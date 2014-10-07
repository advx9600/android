__author__ = 'Administrator'
import sys
import os
import zipfile
from os.path import basename

apk_name=sys.argv[1]
apk_name_zip= apk_name+".zip"
target_file=apk_name+".out.apk"
apk_dir=os.path.dirname(apk_name)
#print apk_name_zip
temp_1_zip=apk_dir+'\\1.zip'
temp_1_apk=apk_dir+"\\1.apk"
os.system("copy "+apk_name+" "+apk_name_zip)
zin =zipfile.ZipFile(apk_name_zip,"r")
zout = zipfile.ZipFile (temp_1_zip, 'w')
for item in zin.infolist():
    buffer = zin.read(item.filename)
    if (item.filename[-17:] != 'META-INF/CERT.RSA' and item.filename[-17:] !='META-INF/CERT.SF'):
        zout.writestr(item, buffer)
zout.close()
zin.close()

os.system("del "+apk_name_zip)
os.system("move "+temp_1_zip+"  "+temp_1_apk)

cur_dirname=os.path.dirname(os.path.realpath(__file__))+"\\lib"

os.system("java -jar "+cur_dirname+"\\signapk.jar "+ cur_dirname+"\\platform.x509.pem "+cur_dirname+"\\platform.pk8 "+ temp_1_apk+" "+target_file)
os.system("del "+temp_1_apk)

raw_input("click enter key to exit")