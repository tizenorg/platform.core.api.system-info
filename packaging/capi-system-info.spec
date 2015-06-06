%bcond_with x
%bcond_with wayland
Name:           capi-system-info
Version:        0.2.0
Release:        0
License:        Apache-2.0
Summary:        A System Information library in Core API
Group:          System/API
Source0:        %{name}-%{version}.tar.gz
Source1001:     %{name}.manifest
Source2001:     tizenid.service
BuildRequires:  cmake
BuildRequires:  pkgconfig(capi-base-common)
BuildRequires:  pkgconfig(dlog)
BuildRequires:  pkgconfig(iniparser)
BuildRequires:  pkgconfig(libxml-2.0)
BuildRequires:  pkgconfig(openssl)
BuildRequires:  pkgconfig(glib-2.0)
%if %{with wayland}
BuildRequires:  pkgconfig(ecore-wayland)
%endif
%if %{with x}
BuildRequires:  pkgconfig(x11)
BuildRequires:  pkgconfig(xi)
BuildRequires:  pkgconfig(xrandr)
%endif

%description


%package devel
Summary:  A System Information library in Core API (Development)
Group:    Development/System
Requires: %{name} = %{version}-%{release}

%description devel


%prep
%setup -q
cp %{SOURCE1001} .

%define config_file_path /etc/config/model-config.xml
%define info_file_path /etc/info.ini
%define os_release_file_path /etc/os-release
%define serial_path /csa/imei/serialno.dat
%define tizen_id_path /opt/home/root/tizenid

%build
MAJORVER=`echo %{version} | awk 'BEGIN {FS="."}{print $1}'`
cmake . -DCMAKE_INSTALL_PREFIX=/usr \
		-DFULLVER=%{version} -DMAJORVER=${MAJORVER} \
%if !%{with x} && %{with wayland}
		-DENABLE_WAYLAND=TRUE \
%endif
		-DCONFIG_FILE_PATH=%{config_file_path} \
		-DINFO_FILE_PATH=%{info_file_path} \
		-DOS_RELEASE_FILE_PATH=%{os_release_file_path} \
		-DSERIAL_PATH=%{serial_path} \
		-DTIZEN_ID_PATH=%{tizen_id_path} \

%__make %{?_smp_mflags}

%install
%make_install
mkdir -p %{buildroot}/etc
cp -f script/make_info_file.sh %{buildroot}/etc/make_info_file.sh

mkdir -p %{buildroot}%{_libdir}/systemd/system/multi-user.target.wants
install -m 0644 %SOURCE2001 %{buildroot}%{_libdir}/systemd/system/tizenid.service
ln -s ../tizenid.service %{buildroot}%{_libdir}/systemd/system/multi-user.target.wants/tizenid.service

%post -p /sbin/ldconfig

%postun -p /sbin/ldconfig


%files
%manifest %{name}.manifest
%license LICENSE.APLv2
%{_libdir}/libcapi-system-info.so.*
%attr(0744,root,-) /etc/make_info_file.sh

#tizenid
%{_bindir}/tizen_id
%{_libdir}/systemd/system/tizenid.service
%{_libdir}/systemd/system/multi-user.target.wants/tizenid.service

%files devel
%manifest %{name}.manifest
%{_includedir}/system/system_info.h
%{_includedir}/system/system_info_type.h
%{_libdir}/pkgconfig/*.pc
%{_libdir}/libcapi-system-info.so
