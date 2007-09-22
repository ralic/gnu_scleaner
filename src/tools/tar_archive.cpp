/* 
 Copyright (c) 2004-2005 Andrew Reading.
 Copyright (c) 2007 Sylvain Beucler.
 Copyright (c) 2007 Jean-Michel Frouin.
------------------------------------------------------
This program is free software; you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation; either version 3 of the License, or
(at your option) any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/

#include "tar_archive.h"
#include <tar.h>
#include <grp.h>
#include <pwd.h>
#include <iostream>
#include <fstream>
#include <sstream>
#include <sys/stat.h>
#include <engine/iprogress.h>

CTarArchive::CTarArchive():
m_FirstFile(false)
{
} 

bool CTarArchive::FillHeader(const std::string& _filename, posix_header& _header)
{
	bool l_ret = false;

#if defined DEBUG
	std::cout << "File header for " << _filename << " :" << '\n';
#endif

	struct stat l_info;

    memset(&_header, 0, 512);

	//Try to stat file.
	if(stat(_filename.c_str(), &l_info) == -1)
	{
		std::cout << "[ERR] : " __FILE__ << "@" << __LINE__ << ": Cannot stat " << _filename.c_str() << '\n';
		l_ret = false;
	}
	else
	{
		std::string l_name(_filename);

#if defined WIN32
        // No UID or GID support for Windows - Hack it up.
        strncpy(_header.mode, "0100777", 8);
        strncpy(_header.uid, "0000000", 8);
        strncpy(_header.gid, "0000000", 8);
#else
        int l_mode = 0;

		//Never reached ?
        if(!(l_info.st_mode & S_IFREG))
        {
            // Not a normal file.  Let's archive it anyway, but warn the user.
            std::cout << "Warning: Archiving non-ordinary file (Linked or Device file?)" << '\n';
        }

        // Add the normal file bit.
        l_mode += 100000;

        //  Find out the file permissions.
        if(l_info.st_mode & TUREAD)
        {
            l_mode += 400;
        }
        if(l_info.st_mode & TUWRITE)
        {
            l_mode += 200;
        }
        if(l_info.st_mode & TUEXEC)
        {
            l_mode += 100;
        }
        if(l_info.st_mode & TGREAD)
        {
            l_mode += 40;
        }
        if(l_info.st_mode & TGWRITE)
        {
            l_mode += 20;
        }
        if(l_info.st_mode & TGEXEC)
        {
            l_mode += 10;
        }
        if(l_info.st_mode & TOREAD)
        {
            l_mode += 4;
        }
        if(l_info.st_mode & TOWRITE)
        {
            l_mode += 2;
        }
        if(l_info.st_mode & TOEXEC)
        {
            l_mode += 1;
        }

		std::string l_modestr(itos(l_mode));
        l_modestr = PadWithZeros(l_modestr, 8);
		strncpy(_header.mode, l_modestr.c_str(), 8);

		//UID
		std::string l_uid(DecToOct(l_info.st_uid));
        l_uid = PadWithZeros(l_uid, 8);
		strncpy(_header.uid, l_uid.c_str(), 8);
		
		//GID
		std::string l_gid(DecToOct(l_info.st_gid));
        l_gid = PadWithZeros(l_gid, 8);
		strncpy(_header.gid, l_gid.c_str(), 8);

    	// File Size.
    	std::string l_size = DecToOct(l_info.st_size);
		l_size = PadWithZeros(l_size, 12);
		strncpy(_header.size, l_size.c_str(), 12);

    	// Time of modification
    	std::string l_mtime = DecToOct(time(0));
		l_mtime = PadWithZeros(l_mtime, 12);
		strncpy(_header.mtime, l_mtime.c_str(), 12);

    	// Type of file.
    	_header.typeflag = REGTYPE;
	
		if (S_ISLNK(l_info.st_mode)) 
		{
			_header.typeflag = SYMTYPE;
		} 
		else
		{ 
			if (S_ISDIR(l_info.st_mode)) 
			{
				_header.typeflag = DIRTYPE;
				l_name += "/";
			}
		}

	    strncpy(_header.name, l_name.c_str(), 100);

    	strncpy(_header.linkname, "", 100);

		// Magic
    	strncpy(_header.magic, TMAGIC, TMAGLEN);

    	strncpy(_header.version, TVERSION, TVERSLEN);

  		// Windows again, let's cheat.
#if defined WIN32
      	strncpy(_header.uname, "root", 32);
      	strncpy(_header.gname, "root", 32);
#else
        passwd* l_pwdFileInfo = getpwuid(l_info.st_uid);
        group* l_grpFileInfo = getgrgid(l_info.st_gid);

        if(!l_pwdFileInfo || !l_grpFileInfo)
        {
            // No entry.  Let's fake it.
            strncpy(_header.uname, _header.uid, 32);
            strncpy(_header.gname, _header.gid, 32);
        }
        else
        {
            strncpy(_header.uname, l_pwdFileInfo->pw_name, 32);
            strncpy(_header.gname, l_grpFileInfo->gr_name, 32);
        }
#endif

    	// Skip devminor and devmajor.
    	strncpy(_header.devmajor, "", 8);
    	strncpy(_header.devminor, "", 8);

    	// Fill the prefix.
    	strncpy(_header.prefix, "", 167);

    	// Initialize our pointer.
		char* l_startheaderPtr = reinterpret_cast<char*>(&_header);
    	char* l_headerPtr ;
		int l_checksum = 0;

    	// Find the checksum.
    	for(l_headerPtr = l_startheaderPtr; (l_headerPtr - l_startheaderPtr) < 512; ++l_headerPtr)
    	{
    	    l_checksum += static_cast<int>(*l_headerPtr);
    	}

    	// Add 256 (2^8)
    	l_checksum += 256;

    	// Get it to Octal.
    	std::string l_checksumstr = DecToOct(l_checksum);
		l_checksumstr = PadWithZeros(l_checksumstr, 8);
    	strncpy(_header.chksum, l_checksumstr.c_str(), 8);

#if defined DEBUG
		std::cout << "Mode (8bytes) : " << _header.mode << '\n';
		std::cout << "UID (8b) : " << _header.uid << '\n';
		std::cout << "GID (8b) : " << _header.gid << '\n';
		std::cout << "Size (12b) : " << _header.size << '\n';
		std::cout << "Time (12b) : " << _header.mtime << '\n';
		std::cout << "Checksum : " << _header.chksum << '\n';
#endif

#endif
		l_ret=true;
	}

	return l_ret;	
}

bool CTarArchive::WriteData(const std::string& _input, const std::string& _output)
{
	bool l_ret = false;

    /*int iAmountToRound = 0;
    static bool bFirstEntry = true;
    std::fstream f_In, f_Out;
    std::string strData = strTarfile;*/

    posix_header l_header;
	FillHeader(_input, l_header);

    std::ofstream l_out(_output.c_str(), std::ios::out | std::ios::binary | std::ios::app | std::ios::ate);

    if(!l_out.good())
    {
        std::cerr << "Error out file!" << _output << '\n';
		l_ret = false;
    }
	else
	{
    	std::ifstream l_in(_input.c_str(), std::ios::in | std::ios::binary);

		if(!l_in.good())
		{
        	std::cerr << "Error in file!" << _input << '\n';
			l_ret = false;
		}
		else
		{
			//Round it to % 512
			if(m_FirstFile)
			{
        		int l_AmountToRound = RoundTo512(l_out.tellp()) - l_out.tellp();

        		for(int i = 0; i < l_AmountToRound; ++i)
        		{
        		    l_out.put('\0');
        		}
			}

			m_FirstFile = true;

			//Write header 
    		l_out.write(reinterpret_cast<char*>(&l_header), sizeof(l_header));

			//Append file content
			char l_char;
			while(l_in.get(l_char))
			{
				l_out.put(l_char);
			}

			l_in.close();
			l_out.close();
		}
	}

    return l_ret;
}

bool CTarArchive::CleanClose(const std::string& _output)
{
	bool l_ret = false;

    std::ofstream l_out(_output.c_str(), std::ios::out | std::ios::binary | std::ios::app | std::ios::ate);

    if(!l_out.good())
    {
        std::cout << "Error out file!" << '\n';
		l_ret = false;
    }
	else
	{
        int l_AmountToRound = RoundTo512(l_out.tellp()) - l_out.tellp();

        for(int i = 0; i < l_AmountToRound; ++i)
        {
            l_out.put('\0');
        }
		l_out.put('\0');
		l_out.put('\0');
		l_out.close();
	}
	return l_ret;
}

bool CTarArchive::Create(std::list<std::string> _filenames, const std::string& _output, IProgressbar* _callback)
{
	bool l_ret = true;
	std::list<std::string>::iterator l_it;
	int l_size = _filenames.size();
	int l_done = 0;
	for(l_it = _filenames.begin(); l_it != _filenames.end(); ++l_it)
	{
		++l_done;
		WriteData(*l_it, _output);
		int l_res = l_done*50/l_size;
		std::string l_mess("Adding ");
		l_mess += *l_it + " to " + _output + " archive.";
		_callback->updateProgress(l_mess, l_res);
#if defined DEBUG
		std::cout << *l_it << " appended to " << _output << '\n';
#endif
	}
	CleanClose(_output);
	return l_ret;
}

//Simplify manipulation :
std::string CTarArchive::itos(int _nb)
{
    std::stringstream l_ret;
    l_ret << _nb;
    return l_ret.str();
}

std::string CTarArchive::DecToOct(int _nb)
{
    std::stringstream l_ret;
    l_ret << std::oct << _nb;
    return l_ret.str();
}

std::string CTarArchive::PadWithZeros(std::string& _s, unsigned int _size)
{
    std::string l_ret;
    int l_size = _size - 1;
    int l_padsize = l_size - _s.size();

    if(_s.size() <= _size)
    {
        for(int i = 0; i < l_padsize; ++i)
        {
            l_ret.append(1, '0');
        }
    }
    else
    {
        return static_cast<std::string>(0);
    }

    l_ret.append(_s);

    return l_ret;
}

int CTarArchive::RoundTo512(int n)
{
    if(n % 512 == 0)
    {
        return n;
    }
    else
    {
        return (n - (n % 512)) + 512;
    }
}
/* vi:set ts=4: */
