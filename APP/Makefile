# This Makefile will help you to be link from VNC server

All:
	route add default gw 192.168.7.1
	route
	x11vnc -bg -o %HOME/.x11vnc.log.%VNCDISPLAY -auth /var/run/gdm/auth-for-gdm*/database -display :0 -forever
