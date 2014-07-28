tempfile=temp2013.zip
targetfile=$1.out.apk
mv $1	$tempfile
zip -d   $tempfile "META-INF/CERT.RSA" "META-INF/CERT.SF"
mv $tempfile  $tempfile.apk

java -jar signapk.jar  platform.x509.pem platform.pk8  $tempfile.apk $targetfile
rm $tempfile.apk

# copy to desktop
#mv $targetfile /mnt/hgfs/Desktop/

#rm $targetfile
