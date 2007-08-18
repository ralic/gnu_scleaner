#This program is free software; you can redistribute it and/or modify
#it under the terms of the GNU General Public License as published by
#the Free Software Foundation; either version 3 of the License, or
#(at your option) any later version.
#
#This program is distributed in the hope that it will be useful,
#but WITHOUT ANY WARRANTY; without even the implied warranty of
#MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
#GNU General Public License for more details.
#
#You should have received a copy of the GNU General Public License
#along with this program.  If not, see <http://www.gnu.org/licenses/>.

#!/bin/bash
timestamp=`date +%d%m%y_%H%M%S`
echo "Tar the folder ..."
tar cvf trunk_$timestamp.tar *
echo "Compression of tar archive ..."
bzip2 trunk_$timestamp.tar 
echo "Moving it to backup folder ..."
mv trunk_$timestamp.tar.bz2 ../.