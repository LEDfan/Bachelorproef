#  This is free software: you can redistribute it and/or modify it
#  under the terms of the GNU General Public License as published by
#  the Free Software Foundation, either version 3 of the License, or
#  any later version.
#  The software is distributed in the hope that it will be useful,
#  but WITHOUT ANY WARRANTY; without even the implied warranty of
#  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
#  GNU General Public License for more details.
#  You should have received a copy of the GNU General Public License
#  along with the software. If not, see <http://www.gnu.org/licenses/>.
#
#  Copyright 2018, Niels Aerens, Thomas Avé, Jan Broeckhove, Tobia De Koninck, Robin Jadoul

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

    def parse(self, response):
        print("Parsing")
        for city in response.css('.wikitable:nth-child(6) tr'):
            arrondisement = city.css('td:nth-child(3) a::text').extract_first()
            if arrondisement is not None:
                arrondisement = str(arrondisement.encode("ascii", 'ignore'))[2:-1]
            pop = city.css('td:nth-child(4)::text').extract_first()
            if pop is not None:
                pop = pop.replace(',', "")
            chieftown = city.css('td:nth-child(2) a::text').extract_first()
            url =  city.css('td:nth-child(2) a::attr(href)').extract_first()
            dep =  city.css('td:nth-child(1) a::text').extract_first()
            print('url', url)
            item = {
                        "chieftown": chieftown,
                        "population": pop,
                        "name": arrondisement,
                        "province": dep
                    }
            req = scrapy.Request(response.urljoin(url), callback=self.parse_prefecture)
            req.meta["item"] = item
            yield req
