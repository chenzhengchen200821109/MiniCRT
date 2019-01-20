#include "minicrt.h"
#include "iostream"

namespace std
{
    stdout_stream::stdout_stream() : ofstream()
    {
        fp = stdout;
    }

    stdout_stream cout;

    ofstream::ofstream() : fp(0)
    {
    
    }

    ofstream::ofstream(const char* filename, ofstream::openmode md) : fp(0)
    {
        open(filename, md);
    }

    ofstream::~ofstream()
    {
        close();
    }

    ofstream& ofstream::operator<<(char c)
    {
        minicrt_fputc(c, fp);
        return *this;
    }

    ofstream& ofstream::operator<<(int n)
    {
        minicrt_fprintf(fp, "%d", n);
        return *this;
    }

    ofstream& ofstream::operator<<(const char* str)
    {
        minicrt_fprintf(fp, "%s", str);
        return *this;
    }

    ofstream& ofstream::operator<<(ofstream& (*manip)(ofstream&))
    {
        return manip(*this);
    }

    void ofstream::open(const char* filename, ofstream::openmode md)
    {
        char mode[4];
        close();
        switch(md)
        {
            case out | trunc:
                minicrt_strcpy(mode, "w");
                break;
            case out | in | trunc:
                minicrt_strcpy(mode, "w+");
            case out | trunc | binary:
                minicrt_strcpy(mode, "wb");
                break;
            case out | in | trunc | binary:
                minicrt_strcpy(mode, "wb+");
        }
        fp = minicrt_fopen(filename, mode);
    }

    void ofstream::close()
    {
        if (fp) {
            minicrt_fclose(fp);
            fp = 0;
        }
    }

    ofstream& ofstream::write(const char* buf, unsigned int size)
    {
        minicrt_fwrite(buf, 1, size, fp);
        return *this;
    }
}
