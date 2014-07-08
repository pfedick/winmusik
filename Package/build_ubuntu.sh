check_debian_package()
{
        PKG=$1
        echo -n "$PKG: "
        dpkg -l | grep $PKG >/dev/null 2>&1
        if [ $? -ne 0 ] ; then
                echo "no"
                MISSING="$MISSING $PKG"
        else
                echo "ok"
        fi
}

debian_write_control ()
{
    (
        echo "Source: $PROGNAME"
        echo "Section: video"
        echo "Priority: optional"
        echo "Maintainer: $MAINTAINER"
        echo "Package: $PROGNAME"
        echo "Version: $VERSION"
        echo "Homepage: $HOMEPAGE"
        echo "Architecture: $PLATFORM"
        echo "Depends: $DEPENDS"
        echo "Installed-Size: 1000"
        echo "Description: $DESCRIPTION"
        cat winmusik/README_en.TXT | while read line
        do
                if [ -z "$line" ] ; then
                    echo " ."
                else
                    echo "  $line"
                fi
        done
        ) > debian/control
}

ubuntu_check_dependencies ()
{
    if [ ! -f ".dependency_checked" ] ; then
            #
            # Check Dependencies
            #
            MISSING=""
            echo "INFO: Checking dependency packages..."
            check_debian_package "libpcre3-dev"
            check_debian_package "libjpeg8-dev"
            check_debian_package "libssl-dev"
            check_debian_package "libcurl4-openssl-dev"
            check_debian_package "libmcrypt-dev"
            check_debian_package "libpng12-dev"
            check_debian_package "libbz2-dev"
            check_debian_package "zlib1g-dev"
            check_debian_package "libcdio-dev"
            check_debian_package "libqt4-dev"
            check_debian_package "qt4-dev-tools"
            check_debian_package "qt4-qmake"
            check_debian_package "nasm"
            check_debian_package "dpkg-dev"
        
            if [ "$MISSING" ] ; then
                echo "ERROR: Missing packages, please run the following command:"
                echo ""
                echo "   sudo apt-get install $MISSING"
                echo ""
                exit 1
            fi
            touch .dependency_checked
    fi 
    echo "INFO: all required packages are installed"
}

ubuntu_configure ()
{
    if [ ! -f ".configure_done" ] ; then
	./configure --with-pcre=/usr --with-libiconv-prefix --with-nasm --with-libmcrypt-prefix \
		--without-libmhash --without-postgresql --with-png --with-jpeg --without-lame \
		--without-ft-prefix --without-libmad --without-lame --without-x --without-mysql \
		--with-libcurl --with-sdl-prefix=$CUR --without-libldns --without-freetds \
		--without-ogg --without-libshout --without-sqlite
	if [ $? -ne 0 ] ; then
		exit 1
	fi
	touch .configure_done
    else
	echo "INFO: configure already done"
    fi
}

ubuntu_make ()
{
    if [ ! -f ".make_done" ] ; then
	make
	if [ $? -ne 0 ] ; then
		exit 1
	fi
	touch .make_done
    else
	echo "INFO: make already done"
    fi
}

if [ "$DISTRIB_ID" != "Ubuntu" ] ; then
	echo "ERROR: This is not an ubuntu system"
	exit 1
fi

ubuntu_check_dependencies
ubuntu_configure
ubuntu_make

echo "INFO: Build Debian-Packet for $DISTRIB_ID $DISTRIB_RELEASE: $DISTRIB_CODENAME"
DISTNAME="$PROGNAME-$VERSION"
rm -rf debian
create_dir debian
create_dir debian/DEBIAN
create_dir debian/usr/bin
create_dir debian/usr/share/applications
create_dir debian/usr/share/pixmaps
create_dir debian/usr/share/doc/$PROGNAME
chmod -R 755 debian
cp winmusik/release/$PROGNAME debian/usr/bin/$PROGNAME
cp winmusik/resources/icon64.png debian/usr/share/pixmaps/$PROGNAME.png
cp winmusik/docs/Userguide_de.pdf debian/usr/share/doc/$PROGNAME

DEPENDS=""
cat WinMusik.desktop | sed "s/Exec=.*/Exec=\/usr\/bin\/$PROGNAME/" | sed "s/Icon=.*/Icon=\/usr\/share\/pixmaps\/$PROGNAME.png/" > debian/usr/share/applications/$PROGNAME.desktop

(
	echo "$PROGNAME ($VERSION) unstable; urgency=low"
	echo ""
	echo " * No upstream changes."
	echo ""
	echo -n "-- $USER   "
	date
	echo ""
	echo "Local variables:"
	echo "mode: debian-changelog"
	echo "End:"
) > debian/changelog

debian_write_control
dpkg-shlibdeps debian/usr/bin/$PROGNAME
DEPENDS=`grep "shlibs:Depends" debian/substvars | sed -e "s/shlibs:Depends=//"`
debian_write_control
cp debian/control debian/DEBIAN
rm -rf debian/control
rm -rf debian/substvars
rm -rf debian/changelog
dpkg -b debian $DISTNAME-$DISTRIB_ID-$DISTRIB_RELEASE-$PLATFORM.deb
if [ $? -ne 0 ] ; then
	echo "ERROR: Could not create debian package"
	exit 1
fi
echo "DONE. Debian package successfully created: $DISTNAME-$DISTRIB_ID-$DISTRIB_RELEASE-$PLATFORM.deb"
echo ""
echo "Install with sudo dpkg -i $DISTNAME-$DISTRIB_ID-$DISTRIB_RELEASE-$PLATFORM.deb"














