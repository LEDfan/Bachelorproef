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
#  Copyright 2018, Niels Aerens, Thomas Avé, Tobia De Koninck, Robin Jadoul

import scrapy

class CityNLSpider(scrapy.Spider):
    name = "cityNL"

    start_urls = [
        'https://nl.wikipedia.org/wiki/Lijst_van_gemeenten_in_het_Waals_Gewest',
    ]

    def parse_gemeente(self, response):
         item = response.meta['item']
         coordurl =  response.css('#text_coordinates > a::attr(href)').extract_first()

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
        for city in response.css('.wikitable > tbody > tr'):
            pop = city.css('td:nth-child(5)::text').extract_first()
            if pop is not None:
                pop = pop.replace("\n", "")
                pop = pop.replace(".", "")
            name = city.css('td:nth-child(1) > a::text').extract_first()
            if name is not None:
                name = str(name.encode("ascii", "ignore"))[2:-1]
            province = city.css('td:nth-child(3)::text').extract_first()
            if province is not None:
                province = province.replace("\n", "")


            item = {"name": name,
                    "province": province,
                    "population": pop,
            }

            gemeenteUrl = city.css('td:nth-child(1) > a::attr(href)').extract_first()
            if gemeenteUrl is not None:
                req = scrapy.Request(response.urljoin(gemeenteUrl), callback=self.parse_gemeente)
                req.meta['item'] =  item
                yield req


            yield None
