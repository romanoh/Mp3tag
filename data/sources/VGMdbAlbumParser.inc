# ###################################################################
# Mp3tag parsing for VGMdb.net, created by dano on 2010-01-20
#
# After 26.06.2020, changes in the commits: https://github.com/romanoh/Mp3tag-Repository
# Last update 30.10.2020
# 
# Copy this file to your sources directory at
# %APPDATA%\Mp3tag\data\sources
# ###################################################################


[ParserScriptAlbum]=...
# ###################################################################
#					A  L  B  U  M
# ###################################################################
Debug "ON" "C:\Users\egual\Desktop\vgmdbalbum.txt"

# Album
outputto "Album"
findline "class=\"albumtitle\""
replace "|" "$verticalBar()"
findinline "class=\"albumtitle\""
findinline ">"
sayuntil "</span"

# Coverurl
outputto "coverurl"
findline "id=\"coverart\""
findinline "style=\"background-image: url("
#get full image
#replace "medium-" ""
#findinline "style=\"background-image: url("
if "'"
	movechar 1
	sayuntil "'"
	
else
	findinline "../"
	sayuntil "\""
endif


# Catalog
outputto "Catalog"
findline "<b>Catalog Number</b>"
joinuntil "</tr>"
findinline "<td width=\"100%\">"
ifnot "N/A"
	if "<span id="
		findinline "#\">"
		sayuntil "</a>"
	else
		regexpreplace "<a[^>]+>" ""
		replace "</a>" ""
		RegexpReplace "\[Extra Credits].*?script>" ""
		findinline "<td width=\"100%\">"
		sayuntil "</td>"
	endif
endif

# Release Date
outputto "Year"
findline "<b>Release Date</b>"
moveline 1
sayregexp "(?<=<td>)\d{4} *(?=</td>)" "" 

if "<td><a "
	findinline "#"
	SayNChars 4
	say "."
	SayNChars 2
	say "."
	SayNChars 2
	
endif


# Publish Format
outputto "Format"
findline "<b>Publish Format</b>"
joinuntil "</tr>"
findinline "<td>"
sayuntil "</td>"

# Media Format
outputto "Mediatype"
findline "<b>Media Format</b>"
joinuntil "</tr>"
findinline "<td>"
sayuntil "</td>"

# Classification (Genre)
outputto "Genre"
findline "<b>Classification</b>"
joinuntil "</tr>"
findinline "<td>"
sayuntil "</td>"


# Publisher
findline "<b>Organizations</b>"
moveline 3 1
outputto "Publisher"
say ""
if "<script type=\"text/javascript\">"
	findline "id=\"publisher_less\""	
	outputto "Publisher"
	findinline "display:inline\">"
	sayuntil "</span>"
else
if "<a href=\"/org/"
	outputto "Publisher"
	findinline "style=\"display:inline\">"
	sayuntil "</span>"
	else 
	outputto "Publisher"
	sayuntil "</td></tr>"
endif
endif

# Composer
findline "Composer"
outputto "Composer"
replace "</a>" ""
regexpreplace "<a href=\"/artist/[0-9 ]+\">" ""
regexpreplace "<span title=\"[^\"]*\" class=\"artistname\" lang=\"en\" style=\"display:inline\">" ""
regexpreplace "<span style=\"display:none\"><em> / </em></span><span title=\"[^\"]*\" class=\"artistname\" lang=\"ja\" style=\"display:none\">([^<]+)</span>" ""
regexpreplace "<a style=\"color: #[^\"]*\" href=\"/artist/[^\"]*\">" ""
replace "</span> " ""
replace "</span>)" ")"
replace "</span>," ","

findinline ">Composer</span></b></span></td><td>"

if "<"
findinline ">"
sayuntil "</td>"
else
sayuntil "</td></tr>"
endif

# Artist
outputto "Artist"
sayoutput "Composer"
IfNotOutput "Composer"
	outputto "Artist"
    sayoutput "Writer"
endif

# Tracks
# findline "<b>Disc 2</b></span>"
findline "class=\"smallfont\"><span class=\"label\">"
do
	outputto "Tracks"
	moveline 1
	findinline "width=\"100%\" colspan=\"2\">"
	moveline 2
	sayuntil "</td>"
	say "|"
	
	outputto "_Length"
	moveline 2
	sayregexp "(?<=<span class=\"time\">)[\d:]+(?=</span>)" ""
	say "|"

	findline "class=\"smallfont\"><span class=\"label\">" 1 1	
while "<td "

# Notes
gotoline 1
outputto "Notes"
findline "<h3>Notes"

	findline "id=\"notes\">"
	regexpreplace "<br />" " "
	replace "<i class='label'>" ""
	replace "</i>" ""
	findinline "id=\"notes\">"
	sayuntil "</div>"

# rating
findline "<span rel=\"rating\""
replace "Rated " ""
outputto "rating"
findinline "class="time\">"
sayuntil "</span>"

# comments
findline "Category"
outputto "comment"
moveline 1
sayuntil "</div>"
joinuntil "<div id=\"preview\"></div>"

if "<div style=\"margin-bottom:"
say " - "
moveline 1
findinline "\"display:inline\">"
sayuntil "</span><span"
else
say ""
endif

# Set Artist = Albumartist
outputto "albumartist"
sayoutput "Artist"

# VGMdb Album URL
outputto "VGMdb Album Url"
sayoutput "CurrentUrl"

# this site dont have artist weird
# Set artistname
#outputto "artist"
#sayoutput "composer"

# ... Customization ...

#set "genre" "Game"

# Fields that you want to remove
#	(to remove a field write: set "field")

