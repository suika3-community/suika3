#!/bin/sh

if [ "$1" == "suika3" ]; then
    if [ -n "`which suika3`" ]; then
        exec suika3;
    fi;
    if flatpak info "vn.suika3.engine" > /dev/null 2>&1; then
        exec flatpak run vn.suika3.engine;
    else
        flatpak install SDK/linux/Suika3-x86_64.flatpak;
        if flatpak info "vn.suika3.engine" > /dev/null 2>&1; then
            exec flatpak run vn.suika3.engine;
        fi;
    fi;
fi

if [ "$1" == "suika3-pack" ]; then
    if [ -n "`which suika3-pack`" ]; then
        exec suika3-pack $(ls -A | grep -vE '^(SDK|suika3.exe|.vscode|assets.arc)$');
    fi;
    if flatpak info "vn.suika3.engine" > /dev/null 2>&1; then
        exec flatpak run --command=suika3-pack vn.suika3.engine $(ls -A | grep -vE '^(SDK|suika3.exe|.vscode|assets.arc)$');
    else
        flatpak install SDK/linux/Suika3-x86_64.flatpak;
        if flatpak info "vn.suika3.engine" > /dev/null 2>&1; then
            exec flatpak run --command=suika3-pack vn.suika3.engine $(ls -A | grep -vE '^(SDK|suika3.exe|.vscode|assets.arc)$');
        fi;
    fi;
fi
