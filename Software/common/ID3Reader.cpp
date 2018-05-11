#include <cstring>
#include "ID3Reader.h"

bool ID3Reader::open(const char *filename) {

    if (!SD.exists(filename)) {
        printf("unable to find %s \n", filename);
        return false;
    }

    _file = SD.open(filename);

    if (!_file) {
        printf("unable to open %s \n", filename);
        return false;
    }

    if(!readHeader()) {
        printf("unable to read header of  %s \n", filename);
        return false;
    }

    switch (_header.major_version) {
        case 3:
        case 4:
            return readTags();

        default:
            break;
    }
    return false;
}

bool ID3Reader::readTags() {
    unsigned char buffer[10];

    bool fullTagUnsynch = (_header.major_version < 4) && ((_header.flags & ID3HeaderFlags_Unsynchronisation) != 0);
    bool extendedHeader = (_header.flags & ID3HeaderFlags_ExtendedHeader != 0);
    uint32_t frameHeaderSize = (_header.major_version < 3 ? 6 : 10);
    bool footerPresent = (_header.flags & ID3HeaderFlags_FooterPresent != 0);

    printf("fullTagUnsynch: %d\n", fullTagUnsynch);
    printf("extendedHeader: %d\n", extendedHeader);
    printf("footerPresent: %d\n", footerPresent);
    printf("frameHeaderSize: %d\n", frameHeaderSize);

    while (_file.position() < _header.tag_size + 10) {

        uint32_t position, offset;
        offset = position = _file.position();

        //printf("offset: %x \t\t\t %d\n", offset, offset);

        char frameId[5];
        frameId[4] = 0;

        if (_file.read(frameId, 4) != 4) {
            return false;
        }
        position += 4;
        printf("frameId: %s \t\t\t\n", frameId);
        // read frame size
        unsigned long frame_length;
        switch (_header.major_version) {
            case 2 :

                if (_file.read(buffer, 3) != 3)
                    return false;
                position += 3;
                frame_length = (buffer[0] << 16) | (buffer[1] << 8) | buffer[2];
                break;

            default:
                if (_file.read(buffer, 4) != 4) {
                    return false;
                }
                position += 4;
                if (_header.major_version < 4)
                    frame_length = bytesToUInt64(buffer);
                else
                    frame_length = readUnsigned7bitsX4(buffer);

                char flags[2];
                if (_file.read(flags, 2) != 2) {
                    return false;
                }
                position += 2;

                break;
        }
        if (frameId[0] == 0)
            return true;

        //printf("frame_length: %x \t\t\t %d\n", frame_length, frame_length);
        bool shouldRead = false;
        if (memcmp(frameId, "TIT2", 4) == 0) shouldRead = true;
        if (memcmp(frameId, "TCON", 4) == 0) shouldRead = true;
        if (memcmp(frameId, "TALB", 4) == 0) shouldRead = true;
        if (memcmp(frameId, "TPE2", 4) == 0) shouldRead = true;

        if (memcmp(frameId, "TCOM", 4) == 0) shouldRead = true;


        if (!shouldRead) {
            //printf("frame_length: %d\n", frame_length);
            if (_header.major_version < 4) {
                position += frame_length - 2;
                _file.seek(frame_length - 2);
            }
            else {
                _file.seek(frame_length);
                position += frame_length;
            }

            continue;
        }

        if (_header.major_version > 2) {

            if (extendedHeader) {
                _file.read(buffer, 4);
                position += 4;

                unsigned long size = (_header.major_version == 3 ? 4u : 0u) + bytesToUInt64(buffer);
                //printf("extendedHeader.size: %x\n", size);

                _file.seek(size);
            }
        }
        char *text = new char[frame_length+1]();
        char *unpadded = text;

        _file.read(text, frame_length);
        position += frame_length;
        text[frame_length] = 0;
        while (unpadded[0] == 0)
            unpadded++;

        if (frameId[0] != 0 && unpadded[0] != 0)
            if (onID3Tag)
                onID3Tag(frameId, unpadded);

        delete[] text;
    }
}

unsigned long ID3Reader::readUnsigned7bitsX4(const unsigned char *buffer) const {
    size_t s1;
    size_t s2;
    size_t s3;
    size_t s4;

    s1 = static_cast< size_t >( buffer[0] );
    s2 = static_cast< size_t >( buffer[1] );
    s3 = static_cast< size_t >( buffer[2] );
    s4 = static_cast< size_t >( buffer[3] );
    s1 &= 0x7F;
    s2 &= 0x7F;
    s3 &= 0x7F;
    s4 &= 0x7F;

    unsigned long header_length = (s1 << 21) | (s2 << 14) | (s3 << 7) | s4;
    return header_length;
}

bool ID3Reader::readHeader() {
    unsigned char buffer[4];
    // 0, 1, 2 = 'ID3'
    if (_file.read(buffer, 3) != 3) {
        return false;
    }

    if (buffer[0] != 'I' || buffer[1] != 'D' || buffer[2] != '3') {
        printf("expecting 'ID3' '%c%c%c' \n", buffer[0], buffer[1], buffer[2]);
        return false;
    }

    // 3, 4 = major_version, revision_number
    if (_file.read(buffer, 2) != 2) {
        return false;
    }
    _header.major_version = buffer[0];
    _header.revision_number = buffer[1];
    printf("major_version: %d\n", _header.major_version);
    //printf("revision_number: %d\n", _header.revision_number);

    // 5 = flags
    if (_file.read(buffer, 1) != 1) {
            return false;
    }
    _header.flags = buffer[0];

    // 6 - 10 = tag_size
    if (_file.read(buffer, 4) != 4) {
        return false;
    }
    _header.tag_size = readUnsigned7bitsX4(buffer);
    printf("header size: %d\n", _header.tag_size );
    return true;
}

unsigned int ID3Reader::bytesToUInt64(unsigned char *buffer) {
    return (buffer[0] << 24) | (buffer[1] << 16)| (buffer[2]  << 8) | buffer[3];
}
