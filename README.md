# Checkpoint/Restore for L4 Genode

## WIKI

  [Link](https://vmbaumgarten4.informatik.tu-muenchen.de/projects/idp-cr/wiki)

## Setup

#### Genode (skip this if you have Genode already installed)
First you have to install Genode and its toolchain and its dependecies:

    sudo apt-get install libSDL-dev tclsh expect byacc qemu genisoimage autoconf2.64 autogen bison flex g++ git gperf libxml2-utils subversion xsltproc gawk syslinux-utils yasm iasl

this takes care of the dependencies. Next you will need the genode toolchain to actually build something. Go to http://sourceforge.net/projects/genode/files/genode-toolchain/ and download the toolchain that belongs to the release of Genode that you want to use. Then run:

    sudo tar xPfj genode-toolchain-<version>-<arch>.tar.bz2
to install the toolchain.

Now get your copy of Genode:

    git clone https://github.com/genodelabs/genode.git
    (cd genode;git checkout 15.08)

after this we will have to prepare some of the packages

    (./genode/tool/ports/prepare_port foc)
    (./genode/tool/ports/prepare_port x86emu)
    (cd genode/repos/libports;make prepare)
    (cd genode/repos/ports;make prepare)

Now genode is prepared.

#### This package

    ./setup.py prepare path_to_genode_folder
You are done

## Run

    ./setup.py run
