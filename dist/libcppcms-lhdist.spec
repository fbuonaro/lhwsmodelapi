%global __requires_exclude ^libcppcms\\.so.*$

Name:       libcppcms-lhdist
Version:    1
Release:    1
Summary:    libcppcms packaged in an RPM
License:    See http://cppcms.com/wikipp/en/page/main

%description
libcppcms packaged in an RPM

%prep
wget https://sourceforge.net/projects/cppcms/files/cppcms/1.2.1/cppcms-1.2.1.tar.bz2
tar -xf cppcms-1.2.1.tar.bz2

%build
cd cppcms-1.2.1
mkdir build
cd build
cmake3 -DCMAKE_INSTALL_PREFIX=/usr ../
make

%install
rm -rf %{buildroot}
mkdir -p %{buildroot}/usr
cd cppcms-1.2.1/build
make install DESTDIR=%{buildroot}
cd %{buildroot}
find ./ -type f -or -type l | grep usr | cut -d '.' -f 2- > %{buildroot}/../libcppcms_install_files.txt
find ./ -type f -or -type l | grep usr | xargs -I{} chmod 777 {}

%files -f %{buildroot}/../libcppcms_install_files.txt

%changelog