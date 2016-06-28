Name:           capi-system-info
Version:        0.2.0
Release:        0
License:        Apache-2.0
Summary:        A System Information library in Core API
Group:          System/API
Source0:        %{name}-%{version}.tar.gz
Source1001:     %{name}.manifest
BuildRequires:  cmake
BuildRequires:  pkgconfig(capi-base-common)
BuildRequires:  pkgconfig(dlog)
BuildRequires:  pkgconfig(iniparser)
BuildRequires:  pkgconfig(libxml-2.0)
BuildRequires:  pkgconfig(openssl)
BuildRequires:  pkgconfig(glib-2.0)
BuildRequires:  pkgconfig(libtzplatform-config)
BuildRequires:  gdbm-devel

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
%define tizen_id_path %{TZ_SYS_ETC}/tizenid
%define db_path %{TZ_SYS_RO_ETC}/system_info_db

%build
MAJORVER=`echo %{version} | awk 'BEGIN {FS="."}{print $1}'`

%cmake . -DCONFIG_FILE_PATH=%{config_file_path} \
		 -DINFO_FILE_PATH=%{info_file_path} \
		 -DMAJORVER=${MAJORVER} \
		 -DFULLVER=%{version} \
		 -DTIZEN_ID_PATH=%{tizen_id_path} \
		 -DDB_PATH=%{db_path}

%__make %{?_smp_mflags}

%install
%make_install
mkdir -p %{buildroot}/etc
cp -f script/make_info_file.sh %{buildroot}/etc/make_info_file.sh

%install_service sysinit.target.wants tizenid.service

%post -p /sbin/ldconfig

%postun -p /sbin/ldconfig

%files
%manifest %{name}.manifest
%license LICENSE
%{_libdir}/libcapi-system-info.so.*
%attr(0744,root,-) /etc/make_info_file.sh
%{_bindir}/system_info_init_db

#tizenid
%{_bindir}/tizen_id

%{_unitdir}/tizenid.service
%{_unitdir}/sysinit.target.wants/tizenid.service

%files devel
%manifest %{name}.manifest
%{_includedir}/system/system_info.h
%{_includedir}/system/system_info_type.h
%{_includedir}/plugin/system_info_intf.h
%{_libdir}/pkgconfig/*.pc
%{_libdir}/libcapi-system-info.so
