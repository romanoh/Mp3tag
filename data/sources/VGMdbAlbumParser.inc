# ###################################################################
# Mp3tag parsing for VGMdb.net, created by dano on 2010-01-20.
#
# After 26.06.2020, changes: https://github.com/romanoh/Mp3tag-Repository
# Last update 22.08.2021
#	-Fix: "Rating" removed as no longer in site.
#	-Fix: "Artist" 
#	-Fix: "Event"
# Update 25.08.2021
#	-Fix: "Event" for when an album as none.
# 
# Copy this file to your sources directory at
# %APPDATA%\Mp3tag\data\sources
# ###################################################################

[ParserScriptAlbum]=...
# ###################################################################
#					A  L  B  U  M
# ###################################################################
#Debug "ON" "C:\Users\%user%\Desktop\mp3tagdebug.txt"
#DebugWriteInput "C:\Users\%user%\Desktop\mp3tag.html"

findline "main page contents"

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

# Event
outputto "Event"
findinline "at " 1 1
if ""
	say ""
else
	sayuntil "("
	findinline ".png' />" 1 1
	say " / "
	sayuntil "</span>"
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

# Publisher-Label
findline "class=\"maincred\">"
findinline "<b>" 1 1
If "Publisher"
	outputto "Publisher"
	findinline "style=\"display:inline\">" 1 1
	sayuntil "<"
else
If "Label"
	outputto "Label"
	findinline "style=\"display:inline\">" 1 1
	sayuntil "<"
else
endif
endif

# Distributor
gotochar 1
OutputTo "Distributor"
findinline "Distributor" 1 1
findinline "display:inline\">" 1 1
sayuntil "<"

# Copyright
gotochar 1
outputto "copyright"
findinline "Phonographic Copyright" 1 1
findinline "display:inline\">" 1 1
sayuntil "<"

# Organizations
gotochar 1
outputto "Organizations"
findinline "Organizations" 1 1
joinuntil "</td>"
findinline "<td>" 1 1
sayuntil "<"

# Retailer
gotochar 1
outputto "Retailer"
findinline "Retailer" 1 1
findinline "display:inline\">" 1 1
sayuntil "<"

# Manufacturer
gotochar 1
outputto "Manufacturer"
findinline "Manufacturer" 1 1
findinline "display:inline\">" 1 1
sayuntil "<"

# Composer
findline "left\">Credits " 1 1
findline "<tr class=\"maincred\">" 1 1
findinline "Composer" 4 1
findinline "width=\"100%\">" 1 1
outputto "Composer"

regexpreplace "<a href=\"/artist/[0-9 ]+\">" ""
regexpreplace "<span title=\"[^\"]*\" class=\"artistname\" lang=\"en\" style=\"display:inline\">" ""
regexpreplace "<span style=\"display:none\"><em> / </em></span><span title=\"[^\"]*\" class=\"artistname\" lang=\"ja\" style=\"display:none\">([^<]+)</span>" ""
regexpreplace "<a style=\"color: #[^\"]*\" href=\"/artist/[^\"]*\">" ""
replace "</span> " ""
replace "</span>)" ""
replace "</span>" ""
replace "</a>" ""
replace "<tr class="maincred"><td nowrap="nowrap"><span class="label"><b>Composer</span></b></span></td><td width="100%">" ""
replace "<tr class="maincred"><td nowrap="nowrap"><span class="label"><b>Composer</b></td><td width="100%">" ""
replace "<tr class="maincred"><td nowrap="nowrap"><span class="label"><b>Music</b></td><td width="100%">" ""
replace "<tr class="maincred"><td nowrap="nowrap"><span class="label"><b>Written by</b></td><td width="100%">" ""
replace "<tr class="maincred"><td nowrap="nowrap"><span class="label"><b>All Music Composed by</b></td><td width="100%">" ""
replace "<tr class="maincred"><td nowrap="nowrap"><span class="label"><b>Music Composed by</b></td><td width="100%">" ""
replace "<span style="color:yellow" title="Uncredited">*" ""

sayuntil "</td></tr>"

# LYRICIST
findinline "Lyricist" 1 1
OutputTo "LYRICIST"
findinline "<td width=\"100%\">" 1 1
sayuntil "</td></tr>"

# Artist
outputto "Artist"
sayoutput "Composer"
IfNotOutput "Composer"
	outputto "Artist"
    sayoutput "Writer"
endif

# Tracks
gotoline 1
findline "left\">Tracklist "
findline "class=\"smallfont\"><span class=\"label\">" 1 1
do
	outputto "Tracks"
	moveline 1 1
	findinline "width=\"100%\" colspan=\"2\">" 1 1
	moveline 2 1
	sayuntil "</td>"
	say "|"
	
	outputto "_Length"
	moveline 2 1
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

# VGMdb Album URL
outputto "VGMdb Album Url"
sayoutput "CurrentUrl"

# here the Artist album is the same as the artist.
# Set Artist = Albumartist
outputto "albumartist"
sayoutput "Artist"

# ... Customization ...

#set "genre" "Game"

# Fields that you want to remove
#	(to remove a field write: set "field")