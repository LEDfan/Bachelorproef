import scrapy

class CityNLSpider(scrapy.Spider):
    name = "cityNL"

    start_urls = [
        'https://en.wikipedia.org/wiki/List_of_arrondissements_of_France',
    ]

    def parse_prefecture(self, response):
         item = response.meta['item']
         coordurl =  response.xpath('//*[@class="geo-dms"]/../../@href').extract_first()
         item["prefecture"] = response.css('.fn::text').extract_first()
         print("prefecture", coordurl)

         if coordurl is not None:
            req = scrapy.Request(response.urljoin(coordurl), callback=self.parse_coord)
            req.meta['item'] =  item
            yield req


    def parse_coord(self, response):
         item = response.meta['item']
         item['latitude']= response.css('.latitude::text').extract_first()
         item['longitude']= response.css('.longitude::text').extract_first()
         print("coord")
         yield item


    def parseArrondissement(self, response):
        name = response.css("#firstHeading::text").extract_first()
        name = name.split(' ')[2]
        item = response.meta['item']

        prefecture = response.css('tr.mergedrow:nth-child(7) > td:nth-child(2) > a:nth-child(1)::attr(href)').extract_first()
        item['arrondisement']= name
        req = scrapy.Request(response.urljoin(prefecture), callback=self.parse_prefecture)
        req.meta['item'] = item
        yield req

    def parseDepartment(self, response):
        name = response.css('#firstHeading::text').extract_first().split(' ')[3]
        for item in response.css(".mw-parser-output > ol:nth-child(2) > li"):
            print("\t CHeckin dept")
            data = {}
            data["department"] = name
            url = item.css("a:nth-child(1)::text").extract_first()
            req = scrapy.Request(response.urljoin(url), callback=self.parseArrondissement)
            req.meta['item'] = data
            yield req


    def parse(self, response):
        print("Parsing")
        for city in response.css('table.wikitable:nth-child(8) tr'):
            print("\tcity")
            url =  city.css('td:nth-child(3) > a::attr(href)').extract_first()
            req = scrapy.Request(response.urljoin(url), callback=self.parseDepartment)
            yield req
