#!/bin/sh

# Check if an argument is provided and if the file exists
if [ -z "$1" ] || [ ! -f "$1" ]; then
    # Display error dialog in English
    zenity --error \
           --title="Suika3 Engine Runtime" \
           --text="No script file specified.\n\nPlease right-click a 'main.ray' file and select 'Open with Suika3', or drag and drop your script onto this application." \
           --width=400
    exit 1
fi

# Get the directory of the provided file
TARGET_DIR=$(dirname "$1")

# Change directory and execute
if cd "$TARGET_DIR"; then
    # Launch the engine with the script filename
    exec /app/bin/suika3 "$(basename "$1")"
else
    zenity --error \
           --title="Suika3 Engine Runtime" \
           --text="Failed to access the directory:\n$TARGET_DIR\n\nPlease check your Flatpak permission settings."
    exit 1
fi
