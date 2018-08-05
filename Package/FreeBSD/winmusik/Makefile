# $FreeBSD$

PORTNAME=		WinMusik
PORTVERSION=		3.x.x
CATEGORIES=		multimedia
MASTER_SITES=		https://www.pfp.de/software/download/
MASTER_SITE_SUBDIR=	software/download
EXTRACT_SUFX=		-src.tar.bz2

MAINTAINER=		patrick@pfp.de
COMMENT=		Music database to maintain songs on various devices

LICENSE=		GPLv3

LIB_DEPENDS=		libjpeg.so:${PORTSDIR}/graphics/jpeg-turbo \
			libpng16.so:${PORTSDIR}/graphics/png \
			libfreetype.so:${PORTSDIR}/print/freetype2 \
			libpcre.so:${PORTSDIR}/devel/pcre \
			libmcrypt.so:${PORTSDIR}/security/libmcrypt \
			libcurl.so:${PORTSDIR}/ftp/curl \
			libcdio.so:${PORTSDIR}/sysutils/libcdio
			
BUILD_DEPENDS=		nasm:${PORTSDIR}/devel/nasm

MAKE_JOBS_SAFE=		yes

USE_QT5=		core gui widgets printsupport xml qmake

PLIST_FILES=		bin/${PORTNAME} \
			share/pixmaps/${PORTNAME}.png \
			share/applications/${PORTNAME}.desktop \
			share/doc/${PORTNAME}/Userguide_de.pdf

USES=			iconv

.include <bsd.port.pre.mk>

do-configure:
	cd ${WRKSRC}; \
	./configure --with-pcre=${LOCALBASE} \
		--with-nasm --with-jpeg --with-png --with-libcurl=${LOCALBASE} \
		--prefix=${LOCALBASE} --with-openssl=/usr \
		--with-libcdio \
		--without-libmicrohttpd --with-libmcrypt-prefix=${LOCALBASE} --without-libmhash \
		--without-mpg123 --without-lame --without-ft-prefix --without-libmad \
		--without-lame --without-x --with-sdl-prefix=${WRKSRC} \
		--without-ogg --without-libshout --without-sqlite \
		--without-mysql --without-postgresql --without-freetds --without-libldns

do-build:
	@(cd ${WRKSRC}; if ! ${SETENV} ${MAKE_ENV} ${MAKE} ${MAKE_FLAGS} ${MAKEFILE} ${_MAKE_JOBS} ${MAKE_ARGS} ; then \
		if [ x != x${BUILD_FAIL_MESSAGE} ] ; then \
			${ECHO_MSG} "===> Compilation failed unexpectedly."; \
			(${ECHO_CMD} ${BUILD_FAIL_MESSAGE}) | ${FMT} 75 79 ; \
		fi; \
		${FALSE}; \
	fi)

do-install:
	${MKDIR} ${STAGEDIR}${PREFIX}/bin ${STAGEDIR}${PREFIX}/share/pixmaps ${STAGEDIR}${PREFIX}/share/applications \
		${STAGEDIR}${PREFIX}/share/doc/${PORTNAME}
	${INSTALL_PROGRAM} ${WRKSRC}/winmusik/release/${PORTNAME} ${STAGEDIR}${PREFIX}/bin/${PORTNAME}
	${INSTALL_DATA} ${WRKSRC}/winmusik/resources/icon64.png ${STAGEDIR}${PREFIX}/share/pixmaps/${PORTNAME}.png
	${INSTALL_DATA} ${WRKSRC}/winmusik/docs/Userguide_de.pdf ${STAGEDIR}${PREFIX}/share/doc/${PORTNAME}
	${INSTALL_DATA} ${WRKSRC}/WinMusik.desktop ${STAGEDIR}${PREFIX}/share/applications/${PORTNAME}.desktop

.include <bsd.port.post.mk>
