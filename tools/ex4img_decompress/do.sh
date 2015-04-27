#./simg2img system.img  newsys.img
#mkdir rom
#sudo mount -o loop newsys.img  rom/
#rm system.img
sudo ./make_ext4fs  -s -l 512M -a tempdir system.img rom/
