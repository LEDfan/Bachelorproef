import scrapy

class CityNLSpider(scrapy.Spider):
    name = "cityNL"

    start_urls = [
        'https://en.wikipedia.org/wiki/List_of_cities_and_towns_in_Germany',
    ]
    def isFloat(self, string):
        try:
            float(string)
            return True
        except ValueError:
            return False

    def parse_gemeente(self, response):
         item = response.meta['item']
         coordurl =  response.xpath('//*[@class="geo-dms"]/../../@href').extract_first()
         item['name'] = response.css('.org > span:nth-child(1)::text').extract_first()
         item['province'] = response.css('tr.mergedrow:nth-child(7) > td:nth-child(2) > a:nth-child(1)::text').extract_first()
         item['population'] = response.xpath("//th[contains(text(),'Population')]/../following-sibling::tr/th[contains(text(), 'Total')]/following-sibling::td/text()").extract_first()
         if item['population'] is not None:
             item['population']= item['population'].replace(',', '')
             pass

         if coordurl is not None:
            req = scrapy.Request(response.urljoin(coordurl), callback=self.parse_coord)
            req.meta['item'] =  item
            yield req


    def parse_coord(self, response):
         item = response.meta['item']
         item['latitude']= response.css('.latitude::text').extract_first()
         item['longitude']= response.css('.longitude::text').extract_first()
         yield item


    def parse(self, response):
        for city in response.css('td li'):
            item = {}

            gemeenteUrl = city.css('a::attr(href)').extract_first()
            if gemeenteUrl is not None:
                req = scrapy.Request(response.urljoin(gemeenteUrl), callback=self.parse_gemeente)
                req.meta['item'] =  item
                yield req


            yield None
