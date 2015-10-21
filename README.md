# edKit - the embedded debugging Kit

edKit is a set of tools to help debugging applications in embedded environments.
This repository contains the agents that must be embedded in the software to
monitor.

## Building

edKit depends on the following libraries:
    - json-c
    - cpputest (optional)
    - libunwind (optional)

Run the following command to build the agents:

    ./autogen.sh
    ./configure
    make
    make install

## Getting Started

### edleak

edleak is the tool that tracks memory consumption. The edleak clients available
in other repositories allow to easily detect memory leaks in long life processes
(daemons). The agent is composed of a library the must be preloaded in the
process to inspect.

For example to check for memory leaks on "./my_target", run:

   $> LD_PRELOAD=/usr/lib/libedleak.so ./my_target

The following environment variables can be used to configure edleak:

    - oaktools_httpd_port : The listening port of the embedded http server. The
    default value is 8080.
    - edkit_fw_file : Sets the analysis filename. The default value is
    "edleak.json".
    - edkit_fw_period : Sets the slice dump period. The default value is 30s.

edleak relies on dladdr to retrieve symbols name. If the target symbols names
are not found by edleak, use the following flags in the compiler:

    - -Wl,-export-dynamic
    - -fno-optimize-sibling-calls
    - -fno-omit-frame-pointer

## Analysis

### Html clients

Html clients have been tested on firefox and chromium. If files are served from
the filesystem, then Chromium must be started with the
"allow-file-access-from-files" option:

   $> chromium-browser --allow-file-access-from-files


