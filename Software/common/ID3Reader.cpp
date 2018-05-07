#include "ID3Reader.h"

bool ID3Reader::open(const char *filename) {

    if (!SD.exists(filename))
        return false;

    _file = SD.open(filename);

    if (!_file)
        return false;

    unsigned char buffer[10];
    if (_file.read(buffer, 3) != 3) {
        return false;
    }

    if (buffer[0] != 'I' || buffer[1] != 'D' || buffer[2] != '3')
        return false;

    if (_file.read(buffer, 2) != 2) {
        return false;
    }

    unsigned char versionMajor = buffer[0];
    unsigned char versionMinor = buffer[1];

    if (_file.read(buffer, 1) != 1) {
        return false;
    }
    unsigned char flags = buffer[1];

    if (_file.read(buffer, 4) != 4) {
        return false;
    }

    std::size_t s1;
    std::size_t s2;
    std::size_t s3;
    std::size_t s4;

    s1   = static_cast< std::size_t >( buffer[ 0 ] );
    s2   = static_cast< std::size_t >( buffer[ 1 ] );
    s3   = static_cast< std::size_t >( buffer[ 2 ] );
    s4   = static_cast< std::size_t >( buffer[ 3 ] );
    s1  &= 0x7F;
    s2  &= 0x7F;
    s3  &= 0x7F;
    s4  &= 0x7F;

    unsigned long header_length = (s1 << 21) | (s2 << 14) | (s3 << 7) | s4;

    while( _file.position() < header_length + 10) {

        char frameId[4];
        if (_file.read(frameId, 4) != 4) {
            return false;
        }

        if (_file.read(buffer, 4) != 4) {
            return false;
        }

        s1   = static_cast< std::size_t >( buffer[ 0 ] );
        s2   = static_cast< std::size_t >( buffer[ 1 ] );
        s3   = static_cast< std::size_t >( buffer[ 2 ] );
        s4   = static_cast< std::size_t >( buffer[ 3 ] );
        s1  &= 0x7F;
        s2  &= 0x7F;
        s3  &= 0x7F;
        s4  &= 0x7F;

        unsigned long frame_length = (s1 << 21) | (s2 << 14) | (s3 << 7) | s4;

        char flags[2];

        if (_file.read(flags, 2) != 2) {
            return false;
        }

        if (memcmp(frameId, "TIT2", 4) == 0) {
            std::cout << "title:  ";
            // title
            char *text = new char[frame_length];
            _file.read(text, frame_length);
            std::cout << text << std::endl;
            delete [] text;
        }
    }

    return true;
}
