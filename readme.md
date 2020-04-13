# Audiobook Tag Scraper

This is my modified custom web source for [mp3tag](https://www.mp3tag.de/en/).  
The original authors are qudo, dano, and seanap https://community.mp3tag.de/t/ws-audible-albums-and-series/41227. 

This helps me make sure all audiobooks are tagged properly, have the correct filenames, and have the proper folder structure.  This ensures consistency across Plex.

This script will set the following tags:

| mp3tag Tag    | Audible.com Value|
| ------------- | ---------------- |
| ALBUM         | Title            |
| SUBTITLE      | Subtitle         |
| ARTIST        | Author           |
| ALBUMARTIST   | Author           |
| COMPOSER      | Narrator         |
| YEAR          | Original Year*   |
| COMMENT       | Publisher's Summary|
| SERIES**      | Series           |
| SERIES-PART** | Series Book #    |
| ALBUMSORT     | %Year% %series% %series-part% - %album%|
| COVER         | Cover Art        |
| DISCNUMBER    | Series Book #    |
| PUBLISHER     | Publisher name   |
| COPYRIGHT     | Copyright name   |
| RATING WMP    | Rating Label     |



   >&ast;*Audible is really bad at providing this data*  
   >&ast;&ast;*Create this tag Tools>Options>Tag Panel>New*  

## To Use:
1. Drop the `Audible.com#Search by Album.src` file in your `%appdata%\mp3tag\data\sources` folder.
2. Pull and set the tags
![alt text](https://i.imgur.com/AjJbUqE.png "Tag Source")
3. Then use my Action to set folder and filename structure.
  
