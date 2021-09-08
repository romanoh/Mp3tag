import re
import ssl
import urllib2
import json
from lxml import html
import time
import datetime


# URLS
SEARCH_URL = 'https://www.javbus.com/search/%s'
BASE_URL = 'https://www.javbus.com'
THUMBNAIL_URL = BASE_URL + '/pics/thumb/'


INITIAL_SCORE = 100


user_agent = 'Mozilla/5.0 (Windows; U; Windows NT 5.1; en-US; rv:1.9.0.7) Gecko/2009021910 Firefox/3.0.7'
headers={'User-Agent':user_agent,}


def Start():
    pass

def dump(obj):
    for attr in dir(obj):
        Log("obj.%s = %r" % (attr, getattr(obj, attr)))
    
def getElementFromUrl(url):
    return html.fromstring(request(url))

def request(url):
    user_agent = 'Mozilla/5.0 (Windows; U; Windows NT 5.1; en-US; rv:1.9.0.7) Gecko/2009021910 Firefox/3.0.7'
    headers={'User-Agent':user_agent,}
    request = urllib2.Request(url,headers=headers)
    ctx = ssl.create_default_context()
    ctx.check_hostname = False
    ctx.verify_mode = ssl.CERT_NONE
    return urllib2.urlopen(request,context=ctx).read()



def elementToString(ele):
    html.tostring(ele, encoding='unicode')


def file_exists(url):
    user_agent = 'Mozilla/5.0 (Windows; U; Windows NT 5.1; en-US; rv:1.9.0.7) Gecko/2009021910 Firefox/3.0.7'
    headers={'User-Agent':user_agent,}
    try:
        response = urllib2.Request(url,headers=headers)
        status_code = urllib2.urlopen(response).getcode()
        if status_code == 200:
            return True
        else:
            return False
    except urllib2.HTTPError:
        return False


def query_string(file_name):
    code_match_pattern1 = '[a-zA-Z]{2,5}[-_][0-9]{3,5}'
    code_match_pattern2 = '([a-zA-Z]{2,5})([0-9]{3,5})'
    re_rules1 = re.compile(code_match_pattern1, flags=re.IGNORECASE)
    re_rules2 = re.compile(code_match_pattern2, flags=re.IGNORECASE)

    file_code1 = re_rules1.findall(file_name)
    file_code2 = re_rules2.findall(file_name)
    if file_code1:
        query = file_code1[0].upper()
    elif file_code2:
        query = file_code2[0][0].upper() + '-' + file_code2[0][1]
    else:
        query = file_name
    return query

class JavAgent(Agent.Movies):
    name = 'JavBus'
    languages = [Locale.Language.English,  Locale.Language.Japanese]
    primary_provider = True
    accepts_from = ['com.plexapp.agents.localmedia']


    def search(self, results, media, lang):
        file_name = media.name.replace(' ', '-')
        query = query_string(file_name)

        # javbus.search(query,results,media,lang)
        try:
            url = str(SEARCH_URL % query)

            for movie in getElementFromUrl(url).xpath('//a[contains(@class,"movie-box")]'):
                
                # curName = movie.text_content().strip()
                # curID = movie.get('href').split('/')[3]
                curID = movie.get('href')
                ShortID = movie.get('href').split('/')[3]
                score = INITIAL_SCORE - Util.LevenshteinDistance(query.replace('-','').lower(), ShortID.replace('-','').lower())
                
                entryScore = int(score)
                curName = movie.xpath('.//img')[0].get("title")

                # curNameEn = translator.translate(curName, dest="en")

                curDateTime = movie.xpath('.//date[2]')[0].text_content().strip()

                curYear = re.search(r"(\d{4})", str(curDateTime)).group(1)

                movName =  ShortID + ' [' + curDateTime + '] ' + curName

                img = movie.xpath('.//img')[0].get("src")
                imgName = img.split('/')[3].replace('.jpg','')

                movID = ShortID + '|' + imgName
                
                results.Append(MetadataSearchResult(id=movID, name=movName, year=curYear, lang=lang, score=entryScore))
                # results.Append(MetadataSearchResult(id=theGuid, name=title, year=year, lang=lang, score=bestHitScore))
                
            results.Sort('score', descending=True)
        except Exception as e: pass

    def update(self, metadata, media, lang):
        i = 0
        url=str(BASE_URL + '/' + str(metadata.id).split("|")[0])
        # url_en=str(BASE_URL + '/en/' + str(metadata.id).split("|")[0])
        movie = getElementFromUrl(url).xpath('//div[@class="container"]')[0]

        movie_en = getElementFromUrl(url.replace('https://www.javbus.com', 'https://www.javbus.com/en')).xpath('//div[@class="container"]')[0]
        metadata.title = media.title
        # html = HTML.ElementFromURL(BASE_URL + '/' + str(metadata.id).split("|")[0])
        # metadata.title = media.title

        #Get Thumbnail image
        # metadata.posters.clear()
        thumbUrl = THUMBNAIL_URL + str(metadata.id).split("|")[1] + '.jpg'
        if file_exists(thumbUrl):
            metadata.posters[thumbUrl] = Proxy.Preview(HTTP.Request(thumbUrl), sort_order=i)
        
        bigImage = THUMBNAIL_URL.replace('thumb','cover') + str(metadata.id).split("|")[1] + '_b.jpg'
        if file_exists(bigImage):
            metadata.art[bigImage] = Proxy.Preview(HTTP.Request(bigImage), sort_order=i)

        # tempsample = movie_en.xpath('.//a[@class="sample-box hoverZoomLink"]//img//text()')[0]
        # metadata.summary = tempsample.xpath('./@title')[0].text_content().strip()

        if movie_en.xpath('.//h3'):
            metadata.title = movie_en.xpath('.//h3')[0].text_content().strip()
            # metadata.summary = movie_en.xpath('.//h3')[0].text_content().strip()
        # metadata.summary = movie♥
                #Studio
        if movie_en.xpath('.//div[@class="row movie"]/div[2]/p[5]/a'):
            metadata.studio = movie_en.xpath('.//div[@class="row movie"]/div[2]/p[5]/a')[0].text_content().strip()
 
            

        # Genres
        genrelist = []
        metadata.genres.clear()   
        if movie_en.xpath('.//div[@class="row movie"]//span[@class="genre"]//label//a'):
            genres = movie_en.xpath('.//div[@class="row movie"]//span[@class="genre"]//label//a//text()')
        for genre in genres:
            genre = genre.strip()
            genrelist.append(genre)
            metadata.genres.add(genre)


             # Release
             #moviedate = movie.xpath('.//small[contains(text(),"released")]/following-sibling::text()[1]')[0].strip()
        if movie_en.xpath('.//span[contains(text(),"Release Date:")]//following-sibling::text()[1]'):
            movRelDT = movie_en.xpath('.//span[contains(text(),"Release Date:")]//following-sibling::text()[1]')[0].strip()
            # metadata.summary = movRelDT
            try:
                metadata.originally_available_at = Datetime.ParseDate(movRelDT).date()
                metadata.year = metadata.originally_available_at.year
            except: pass  

            #actors
            metadata.roles.clear()
        for actor in  movie_en.xpath('//div[@class="row movie"]//ul//div[@class="star-name"]//a'):
            elementToString(actor)
            # TODO read actor info in another page
            #img = PIC_BASE_URL + actor.xpath('.//img')[0].get("src")
            role = metadata.roles.new()
            role.name = actor.xpath('./@title')[0]
            upperurl = actor.xpath('./@href')[0]
            upperurl = upperurl.replace("https://www.javbus.com/en/star","https://www.javbus.com/pics/actress")
            upperurl = upperurl + '_a.jpg'
            role.photo = upperurl
 
        try:
            metadata.collections.clear()
            if movie_en.xpath('.//span[contains(text(),"Label:")]//following-sibling::a'):
                series = movie_en.xpath('.//span[contains(text(),"Label:")]//following-sibling::a')[0].text_content().strip()
                # series = HTML.StringFromElement(series).text_content().strip()
                metadata.collections.add(series)
                # metadata.summary = series
        except: pass

        # Director
        try:
            metadata.directors.clear()
            if movie_en.xpath('.//span[contains(text(),"Director:")]//following-sibling::a'):
                htmldirector = movie_en.xpath('.//span[contains(text(),"Director:")]//following-sibling::a')[0].text_content().strip()
                if (len(htmldirector) > 0):
                    director = metadata.directors.new()
                    director.name = htmldirector
        except: pass


            
        try:
            if movie.xpath('.//div[@id="sample-waterfall"]//a'):
                
                for sampleimage in movie.xpath('.//div[@id="sample-waterfall"]//a'):
                    # metadata.summary = sampleimage.xpath.get("href")
                    i = i+1
                    background = sampleimage.get('href')
                    # metadata.art[sample] = Proxy.Preview(HTTP.Request(sample), sort_order=i+1)
                    metadata.art[background] = Proxy.Preview(HTTP.Request(background), sort_order = i)
                    # metadata.art[sample] = Proxy.Preview(HTTP.Request(sample), sort_order=i+1)
        except: pass
