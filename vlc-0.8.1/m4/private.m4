dnl  Private VLC macros - generated by bootstrap

dnl  The required AM_CONDITIONAL calls
dnl  XXX: too many conditionals make the build very slow, disabled them
AC_DEFUN([VLC_CONDITIONALS], [
  AM_CONDITIONAL(UNTRUE, false)
])
dnl  The required AC_SUBST calls
AC_DEFUN([VLC_SUBSTS], [
])
dnl  The required AC_OUTPUT calls
dnl  XXX: this feature is only supported starting from automake-1.7
AC_DEFUN([VLC_MAKEFILES], [AC_OUTPUT([
])])
dnl  Helper macro for vlc-config generation
AC_DEFUN([VLC_CONFIG_HELPER], [
  cat >> vlc-config.in << BLAH
    access_file) list="\${list} access/libaccess_file" ;;
    access_directory) list="\${list} access/libaccess_directory" ;;
    access_udp) list="\${list} access/libaccess_udp" ;;
    access_tcp) list="\${list} access/libaccess_tcp" ;;
    access_http) list="\${list} access/libaccess_http" ;;
    access_ftp) list="\${list} access/libaccess_ftp" ;;
    dvdnav) list="\${list} access/libdvdnav" ;;
    dvdread) list="\${list} access/libdvdread" ;;
    slp) list="\${list} access/libslp" ;;
    cdda) list="\${list} access/libcdda" ;;
    dshow) list="\${list} access/dshow/libdshow" ;;
    dvb) list="\${list} access/dvb/libdvb" ;;
    access_mms) list="\${list} access/mms/libaccess_mms" ;;
    pvr) list="\${list} access/pvr/libpvr" ;;
    satellite) list="\${list} access/satellite/libsatellite" ;;
    v4l) list="\${list} access/v4l/libv4l" ;;
    cddax) list="\${list} access/cdda/libcddax" ;;
    vcd) list="\${list} access/vcd/libvcd" ;;
    vcdx) list="\${list} access/vcdx/libvcdx" ;;
    screen) list="\${list} access/screen/libscreen" ;;
    access_output_dummy) list="\${list} access_output/libaccess_output_dummy" ;;
    access_output_file) list="\${list} access_output/libaccess_output_file" ;;
    access_output_udp) list="\${list} access_output/libaccess_output_udp" ;;
    access_output_http) list="\${list} access_output/libaccess_output_http" ;;
    equalizer) list="\${list} audio_filter/libequalizer" ;;
    normvol) list="\${list} audio_filter/libnormvol" ;;
    audio_format) list="\${list} audio_filter/libaudio_format" ;;
    trivial_channel_mixer) list="\${list} audio_filter/channel_mixer/libtrivial_channel_mixer" ;;
    simple_channel_mixer) list="\${list} audio_filter/channel_mixer/libsimple_channel_mixer" ;;
    headphone_channel_mixer) list="\${list} audio_filter/channel_mixer/libheadphone_channel_mixer" ;;
    float32tos16) list="\${list} audio_filter/converter/libfloat32tos16" ;;
    float32tos8) list="\${list} audio_filter/converter/libfloat32tos8" ;;
    float32tou16) list="\${list} audio_filter/converter/libfloat32tou16" ;;
    float32tou8) list="\${list} audio_filter/converter/libfloat32tou8" ;;
    a52tospdif) list="\${list} audio_filter/converter/liba52tospdif" ;;
    a52tofloat32) list="\${list} audio_filter/converter/liba52tofloat32" ;;
    dtstospdif) list="\${list} audio_filter/converter/libdtstospdif" ;;
    dtstofloat32) list="\${list} audio_filter/converter/libdtstofloat32" ;;
    fixed32tos16) list="\${list} audio_filter/converter/libfixed32tos16" ;;
    s16tofixed32) list="\${list} audio_filter/converter/libs16tofixed32" ;;
    fixed32tofloat32) list="\${list} audio_filter/converter/libfixed32tofloat32" ;;
    s16tofloat32) list="\${list} audio_filter/converter/libs16tofloat32" ;;
    s16tofloat32swab) list="\${list} audio_filter/converter/libs16tofloat32swab" ;;
    s8tofloat32) list="\${list} audio_filter/converter/libs8tofloat32" ;;
    u8tofixed32) list="\${list} audio_filter/converter/libu8tofixed32" ;;
    u8tofloat32) list="\${list} audio_filter/converter/libu8tofloat32" ;;
    mpgatofixed32) list="\${list} audio_filter/converter/libmpgatofixed32" ;;
    trivial_resampler) list="\${list} audio_filter/resampler/libtrivial_resampler" ;;
    ugly_resampler) list="\${list} audio_filter/resampler/libugly_resampler" ;;
    linear_resampler) list="\${list} audio_filter/resampler/liblinear_resampler" ;;
    bandlimited_resampler) list="\${list} audio_filter/resampler/libbandlimited_resampler" ;;
    coreaudio_resampler) list="\${list} audio_filter/resampler/libcoreaudio_resampler" ;;
    trivial_mixer) list="\${list} audio_mixer/libtrivial_mixer" ;;
    float32_mixer) list="\${list} audio_mixer/libfloat32_mixer" ;;
    spdif_mixer) list="\${list} audio_mixer/libspdif_mixer" ;;
    alsa) list="\${list} audio_output/libalsa" ;;
    arts) list="\${list} audio_output/libarts" ;;
    coreaudio) list="\${list} audio_output/libcoreaudio" ;;
    aout_directx) list="\${list} audio_output/libaout_directx" ;;
    esd) list="\${list} audio_output/libesd" ;;
    aout_file) list="\${list} audio_output/libaout_file" ;;
    oss) list="\${list} audio_output/liboss" ;;
    aout_sdl) list="\${list} audio_output/libaout_sdl" ;;
    waveout) list="\${list} audio_output/libwaveout" ;;
    hd1000a) list="\${list} audio_output/libhd1000a" ;;
    portaudio) list="\${list} audio_output/libportaudio" ;;
    a52) list="\${list} codec/liba52" ;;
    cinepak) list="\${list} codec/libcinepak" ;;
    dts) list="\${list} codec/libdts" ;;
    flacdec) list="\${list} codec/libflacdec" ;;
    lpcm) list="\${list} codec/liblpcm" ;;
    araw) list="\${list} codec/libaraw" ;;
    vorbis) list="\${list} codec/libvorbis" ;;
    tarkin) list="\${list} codec/libtarkin" ;;
    theora) list="\${list} codec/libtheora" ;;
    tremor) list="\${list} codec/libtremor" ;;
    speex) list="\${list} codec/libspeex" ;;
    adpcm) list="\${list} codec/libadpcm" ;;
    mpeg_audio) list="\${list} codec/libmpeg_audio" ;;
    libmpeg2) list="\${list} codec/liblibmpeg2" ;;
    rawvideo) list="\${list} codec/librawvideo" ;;
    quicktime) list="\${list} codec/libquicktime" ;;
    subsdec) list="\${list} codec/libsubsdec" ;;
    faad) list="\${list} codec/libfaad" ;;
    dvbsub) list="\${list} codec/libdvbsub" ;;
    mash) list="\${list} codec/libmash" ;;
    x264) list="\${list} codec/libx264" ;;
    toolame) list="\${list} codec/libtoolame" ;;
    cmml) list="\${list} codec/cmml/libcmml" ;;
    dmo) list="\${list} codec/dmo/libdmo" ;;
    ffmpeg) list="\${list} codec/ffmpeg/libffmpeg" ;;
    ffmpegaltivec) list="\${list} codec/ffmpeg/libffmpegaltivec" ;;
    svcdsub) list="\${list} codec/ogt/libsvcdsub" ;;
    cvdsub) list="\${list} codec/ogt/libcvdsub" ;;
    spudec) list="\${list} codec/spudec/libspudec" ;;
    gestures) list="\${list} control/libgestures" ;;
    showintf) list="\${list} control/libshowintf" ;;
    http) list="\${list} control/libhttp" ;;
    telnet) list="\${list} control/libtelnet" ;;
    netsync) list="\${list} control/libnetsync" ;;
    ntservice) list="\${list} control/libntservice" ;;
    joystick) list="\${list} control/libjoystick" ;;
    hotkeys) list="\${list} control/libhotkeys" ;;
    lirc) list="\${list} control/liblirc" ;;
    rc) list="\${list} control/librc" ;;
    rtci) list="\${list} control/librtci" ;;
    corba) list="\${list} control/corba/libcorba" ;;
    a52sys) list="\${list} demux/liba52sys" ;;
    dtssys) list="\${list} demux/libdtssys" ;;
    flac) list="\${list} demux/libflac" ;;
    ogg) list="\${list} demux/libogg" ;;
    m3u) list="\${list} demux/libm3u" ;;
    demuxdump) list="\${list} demux/libdemuxdump" ;;
    rawdv) list="\${list} demux/librawdv" ;;
    au) list="\${list} demux/libau" ;;
    wav) list="\${list} demux/libwav" ;;
    aac) list="\${list} demux/libaac" ;;
    mkv) list="\${list} demux/libmkv" ;;
    livedotcom) list="\${list} demux/liblivedotcom" ;;
    nsv) list="\${list} demux/libnsv" ;;
    real) list="\${list} demux/libreal" ;;
    ts) list="\${list} demux/libts" ;;
    ps) list="\${list} demux/libps" ;;
    mod) list="\${list} demux/libmod" ;;
    pva) list="\${list} demux/libpva" ;;
    aiff) list="\${list} demux/libaiff" ;;
    sgimb) list="\${list} demux/libsgimb" ;;
    mjpeg) list="\${list} demux/libmjpeg" ;;
    subtitle) list="\${list} demux/libsubtitle" ;;
    vobsub) list="\${list} demux/libvobsub" ;;
    asf) list="\${list} demux/asf/libasf" ;;
    avi) list="\${list} demux/avi/libavi" ;;
    mp4) list="\${list} demux/mp4/libmp4" ;;
    m4a) list="\${list} demux/mpeg/libm4a" ;;
    m4v) list="\${list} demux/mpeg/libm4v" ;;
    mpga) list="\${list} demux/mpeg/libmpga" ;;
    mpgv) list="\${list} demux/mpeg/libmpgv" ;;
    h264) list="\${list} demux/mpeg/libh264" ;;
    playlist) list="\${list} demux/playlist/libplaylist" ;;
    id3) list="\${list} demux/util/libid3" ;;
    id3tag) list="\${list} demux/util/libid3tag" ;;
    ncurses) list="\${list} gui/libncurses" ;;
    beos) list="\${list} gui/beos/libbeos" ;;
    pda) list="\${list} gui/pda/libpda" ;;
    gtk) list="\${list} gui/gtk/libgtk" ;;
    gnome) list="\${list} gui/gtk/libgnome" ;;
    gtk2) list="\${list} gui/gtk2/libgtk2" ;;
    gnome2) list="\${list} gui/gtk2/libgnome2" ;;
    kde) list="\${list} gui/kde/libkde" ;;
    macosx) list="\${list} gui/macosx/libmacosx" ;;
    qnx) list="\${list} gui/qnx/libqnx" ;;
    qt) list="\${list} gui/qt/libqt" ;;
    skins2) list="\${list} gui/skins2/libskins2" ;;
    wxwindows) list="\${list} gui/wxwindows/libwxwindows" ;;
    gtk_main) list="\${list} misc/libgtk_main" ;;
    gnome_main) list="\${list} misc/libgnome_main" ;;
    gtk2_main) list="\${list} misc/libgtk2_main" ;;
    gnome2_main) list="\${list} misc/libgnome2_main" ;;
    sap) list="\${list} misc/libsap" ;;
    screensaver) list="\${list} misc/libscreensaver" ;;
    qte_main) list="\${list} misc/libqte_main" ;;
    freetype) list="\${list} misc/libfreetype" ;;
    logger) list="\${list} misc/liblogger" ;;
    vod_rtsp) list="\${list} misc/libvod_rtsp" ;;
    gnutls) list="\${list} misc/libgnutls" ;;
    svg) list="\${list} misc/libsvg" ;;
    dummy) list="\${list} misc/dummy/libdummy" ;;
    memcpy) list="\${list} misc/memcpy/libmemcpy" ;;
    memcpymmx) list="\${list} misc/memcpy/libmemcpymmx" ;;
    memcpymmxext) list="\${list} misc/memcpy/libmemcpymmxext" ;;
    memcpy3dn) list="\${list} misc/memcpy/libmemcpy3dn" ;;
    memcpyaltivec) list="\${list} misc/memcpy/libmemcpyaltivec" ;;
    ipv4) list="\${list} misc/network/libipv4" ;;
    ipv6) list="\${list} misc/network/libipv6" ;;
    test1) list="\${list} misc/testsuite/libtest1" ;;
    test2) list="\${list} misc/testsuite/libtest2" ;;
    test3) list="\${list} misc/testsuite/libtest3" ;;
    test4) list="\${list} misc/testsuite/libtest4" ;;
    export) list="\${list} misc/playlist/libexport" ;;
    mux_dummy) list="\${list} mux/libmux_dummy" ;;
    mux_avi) list="\${list} mux/libmux_avi" ;;
    mux_ogg) list="\${list} mux/libmux_ogg" ;;
    mux_mp4) list="\${list} mux/libmux_mp4" ;;
    mux_asf) list="\${list} mux/libmux_asf" ;;
    mux_wav) list="\${list} mux/libmux_wav" ;;
    mux_mpjpeg) list="\${list} mux/libmux_mpjpeg" ;;
    mux_ps) list="\${list} mux/mpeg/libmux_ps" ;;
    mux_ts) list="\${list} mux/mpeg/libmux_ts" ;;
    packetizer_copy) list="\${list} packetizer/libpacketizer_copy" ;;
    packetizer_mpegvideo) list="\${list} packetizer/libpacketizer_mpegvideo" ;;
    packetizer_mpeg4video) list="\${list} packetizer/libpacketizer_mpeg4video" ;;
    packetizer_mpeg4audio) list="\${list} packetizer/libpacketizer_mpeg4audio" ;;
    packetizer_h264) list="\${list} packetizer/libpacketizer_h264" ;;
    stream_out_dummy) list="\${list} stream_out/libstream_out_dummy" ;;
    stream_out_description) list="\${list} stream_out/libstream_out_description" ;;
    stream_out_standard) list="\${list} stream_out/libstream_out_standard" ;;
    stream_out_transcode) list="\${list} stream_out/libstream_out_transcode" ;;
    stream_out_duplicate) list="\${list} stream_out/libstream_out_duplicate" ;;
    stream_out_es) list="\${list} stream_out/libstream_out_es" ;;
    stream_out_display) list="\${list} stream_out/libstream_out_display" ;;
    stream_out_gather) list="\${list} stream_out/libstream_out_gather" ;;
    stream_out_rtp) list="\${list} stream_out/libstream_out_rtp" ;;
    stream_out_transrate) list="\${list} stream_out/transrate/libstream_out_transrate" ;;
    i420_rgb) list="\${list} video_chroma/libi420_rgb" ;;
    i420_rgb_mmx) list="\${list} video_chroma/libi420_rgb_mmx" ;;
    i420_yuy2) list="\${list} video_chroma/libi420_yuy2" ;;
    i420_yuy2_mmx) list="\${list} video_chroma/libi420_yuy2_mmx" ;;
    i420_yuy2_altivec) list="\${list} video_chroma/libi420_yuy2_altivec" ;;
    i422_yuy2) list="\${list} video_chroma/libi422_yuy2" ;;
    i422_yuy2_mmx) list="\${list} video_chroma/libi422_yuy2_mmx" ;;
    i420_ymga) list="\${list} video_chroma/libi420_ymga" ;;
    i420_ymga_mmx) list="\${list} video_chroma/libi420_ymga_mmx" ;;
    transform) list="\${list} video_filter/libtransform" ;;
    invert) list="\${list} video_filter/libinvert" ;;
    adjust) list="\${list} video_filter/libadjust" ;;
    distort) list="\${list} video_filter/libdistort" ;;
    wall) list="\${list} video_filter/libwall" ;;
    clone) list="\${list} video_filter/libclone" ;;
    crop) list="\${list} video_filter/libcrop" ;;
    motionblur) list="\${list} video_filter/libmotionblur" ;;
    logo) list="\${list} video_filter/liblogo" ;;
    deinterlace) list="\${list} video_filter/libdeinterlace" ;;
    blend) list="\${list} video_filter/libblend" ;;
    scale) list="\${list} video_filter/libscale" ;;
    time) list="\${list} video_filter/libtime" ;;
    marq) list="\${list} video_filter/libmarq" ;;
    swscale) list="\${list} video_filter/swscale/libswscale" ;;
    aa) list="\${list} video_output/libaa" ;;
    caca) list="\${list} video_output/libcaca" ;;
    fb) list="\${list} video_output/libfb" ;;
    ggi) list="\${list} video_output/libggi" ;;
    glide) list="\${list} video_output/libglide" ;;
    vout_sdl) list="\${list} video_output/libvout_sdl" ;;
    svgalib) list="\${list} video_output/libsvgalib" ;;
    wingdi) list="\${list} video_output/libwingdi" ;;
    mga) list="\${list} video_output/libmga" ;;
    hd1000v) list="\${list} video_output/libhd1000v" ;;
    snapshot) list="\${list} video_output/libsnapshot" ;;
    opengl) list="\${list} video_output/libopengl" ;;
    vout_directx) list="\${list} video_output/directx/libvout_directx" ;;
    glwin32) list="\${list} video_output/directx/libglwin32" ;;
    qte) list="\${list} video_output/qte/libqte" ;;
    x11) list="\${list} video_output/x11/libx11" ;;
    xvideo) list="\${list} video_output/x11/libxvideo" ;;
    glx) list="\${list} video_output/x11/libglx" ;;
    goom) list="\${list} visualization/libgoom" ;;
    xosd) list="\${list} visualization/libxosd" ;;
    visual) list="\${list} visualization/visual/libvisual" ;;
    galaktos) list="\${list} visualization/galaktos/libgalaktos" ;;
BLAH
])