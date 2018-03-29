import scrapy

class CitySpider(scrapy.Spider):
    name = "city"

    start_urls = [
        'https://nl.wikipedia.org/wiki/Lijst_van_gemeenten_in_het_Vlaams_Gewest',
    ]

    def parse_nis(self, response):
        nis = response.xpath('//td/a[contains(text(),"NIS")]/../following-sibling::td[1]/text()').extract_first()
        yield {'nis': nis,
              'name': response.meta['parent_name'],
              "coordinates": response.xpath('//td/a[contains(text(),"rdinaten")]/../following-sibling::td[1]//a/text()').extract_first()}

    def parse(self, response):
        for city in response.css('.wikitable > tbody > tr'):
            item = {"parent_name": city.css('td:nth-child(1) > a::text').extract_first(),
                    "parent_area": city.css('td:nth-child(4)::text').extract_first(),
                    "parent_population": city.css('td:nth-child(5)::text').extract_first(),
            }

            nisUrl = city.css('td:nth-child(1) > a::attr(href)').extract_first()
            if nisUrl is not None:
                yield scrapy.Request(response.urljoin(nisUrl), callback=self.parse_nis, meta=item)

            main = city.css('td:nth-child(6)::text').extract_first()

            for deel in city.css('td:nth-child(6) > a::attr(href)').extract():
                request = scrapy.Request(response.urljoin(deel), callback=self.parse_submunicipality)
                request.meta['item'] = item
                yield request

            if main is not None:
                name = main.replace(',', '').strip()
                if name != "":

                    mainItem = {
                        "name": name,
                        "main": True
                    }

                    yield mainItem

    def parse_submunicipality(self, response):
        item = response.meta['item']

        item['child'] = {
            "name": response.css('#firstHeading::text').extract_first(),
            "area": response.xpath('//td/a[contains(text(),"Oppervlakte")]/../following-sibling::td[1]/text()').extract_first(),
            "population": response.xpath('//td/a[contains(text(),"Inwoners")]/../following-sibling::td[1]/text()').extract_first(),
            "coordinates": response.xpath('//td/a[contains(text(),"rdinaten")]/../following-sibling::td[1]//a/text()').extract_first(),
            "province": response.xpath('//td/a[contains(text(),"Provincie")]/../following-sibling::td[1]/a/text()').extract_first()
        }

        yield item
