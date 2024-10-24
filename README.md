# Simple WAV recorder/player utilities

They depends on:

- pulseaudio library

It includes an implementation to read and write WAV files, based on the
description included in the below link:

<https://docs.fileformat.com/audio/wav/>

To record an audio:

    $ ./recorder FILENAME.wav
    
To play an audio:

    $ ./player FILENAME.wav
