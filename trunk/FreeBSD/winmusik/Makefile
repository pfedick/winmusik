# $FreeBSD$

PORTNAME=           WinMusik
PORTVERSION=        3.x.x
CATEGORIES=         multimedia
MASTER_SITES=       ${MASTER_SITE_SOURCEFORGE}
MASTER_SITE_SUBDIR= winmusik/client/Sourcecode
EXTRACT_SUFX=       -src.tar.bz2

MAINTAINER=         patrick@pfp.de
COMMENT=            Music database to maintain songs on various devices

LICENSE=            GPLv3

LIB_DEPENDS=        jpeg:${PORTSDIR}/graphics/jpeg \
                    png15:${PORTSDIR}/graphics/png \
                    freetype:${PORTSDIR}/print/freetype2 \
                    iconv:${PORTSDIR}/converters/libiconv \
                    pcre:${PORTSDIR}/devel/pcre \
                    mcrypt:${PORTSDIR}/security/libmcrypt \
                    curl:${PORTSDIR}/ftp/curl \
                    iconv:${PORTSDIR}/converters/libiconv
BUILD_DEPENDS=      nasm:${PORTSDIR}/devel/nasm

MAKE_JOBS_SAFE=     yes

USE_QT4=            corelib gui qmake

PLIST_FILES=        bin/WinMusik \
                    share/pixmaps/WinMusik.png \
                    share/applications/WinMusik.desktop \
                    share/doc/WinMusik/Userguide_de.pdf

.include <bsd.port.pre.mk>

do-configure:
	cd ${WRKSRC}; ${MKDIR} bin include lib; cd  ${WRKSRC}/ppl6; \
	./configure --with-pcre=${LOCALBASE} --with-libiconv-prefix=${LOCALBASE} \
		--with-nasm --with-jpeg --with-png --with-libcurl=${LOCALBASE} \
		--prefix=${WRKSRC} --with-openssl=/usr \
		--without-libmicrohttpd --with-libmcrypt-prefix=${LOCALBASE} --without-libmhash \
		--without-mpg123 --without-lame --without-ft-prefix --without-libmad \
		--without-lame --without-x --with-sdl-prefix=${WRKSRC} \
		--without-ogg --without-libshout --without-sqlite \
		--without-mysql --without-postgresql --without-freetds --without-libldns

do-build:
	@(cd ${WRKSRC}/ppl6; if ! ${SETENV} ${MAKE_ENV} ${MAKE} ${MAKE_FLAGS} ${MAKEFILE} ${_MAKE_JOBS} ${MAKE_ARGS} ${ALL_TARGET}; then \
		if [ x != x${BUILD_FAIL_MESSAGE} ] ; then \
			${ECHO_MSG} "===> Compilation failed unexpectedly."; \
			(${ECHO_CMD} ${BUILD_FAIL_MESSAGE}) | ${FMT} 75 79 ; \
		fi; \
		${FALSE}; \
	fi)
	@(PATH="${WRKSRC}/bin:${PATH}"; export PATH; cd ${WRKSRC}/WinMusik; \
	qmake-qt4; \
	if ! ${SETENV} ${MAKE_ENV} ${MAKE} ${MAKE_FLAGS} ${MAKEFILE} ${_MAKE_JOBS} ${MAKE_ARGS} ${ALL_TARGET}; then \
		if [ x != x${BUILD_FAIL_MESSAGE} ] ; then \
			${ECHO_MSG} "===> Compilation failed unexpectedly."; \
			(${ECHO_CMD} ${BUILD_FAIL_MESSAGE}) | ${FMT} 75 79 ; \
		fi; \
		${FALSE}; \
	fi)

do-install:
	${MKDIR} ${PREFIX}/bin ${PREFIX}/share/pixmaps ${PREFIX}/share/applications ${PREFIX}/share/doc/${PORTNAME}
	${INSTALL_PROGRAM} ${WRKSRC}/WinMusik/release/${PORTNAME} ${PREFIX}/bin
	${INSTALL_DATA} ${WRKSRC}/WinMusik/docs/Userguide_de.pdf ${PREFIX}/share/doc/${PORTNAME}
	${INSTALL_DATA} ${WRKSRC}/WinMusik/resources/icon64.png ${PREFIX}/share/pixmaps/${PORTNAME}.png
	${INSTALL_DATA} ${FILESDIR}/WinMusik.desktop ${PREFIX}/share/applications/${PORTNAME}.desktop

.include <bsd.port.post.mk>
