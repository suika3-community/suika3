#!/bin/sh

if [ "$1" = "suika3" ]; then
    BASE="$(readlink -f .)";
    FILE="$BASE/SDK/macos/suika3";
    xattr -c "$FILE";
    chmod +x "$FILE";
    if "$FILE"; then
        exit 1;
    fi;
    exit 0;
fi

if [ "$1" = "suika3-pack" ]; then
    BASE="$(readlink -f .)";
    FILE="$BASE/SDK/macos/suika3-pack";
    xattr -c "$FILE";
    chmod +x "$FILE";
    if "$FILE" $(ls -A | grep -vE '^(SDK|assets.arc|suika3.exe|.vscode)$'); then
        exit 1;
    fi;
    exit 0;
fi
