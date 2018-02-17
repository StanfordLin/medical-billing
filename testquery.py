from bs4 import BeautifulSoup
import urllib.request

class CodeSearch:
	def __init__(self, code):
		self.code = code
		self.search_url = "https://www.healthcarebluebook.com/page_SearchResults.aspx?SearchTerms=" + code + "&tab=ShopForCare"
		self.get_price_page()
		self.get_price()

	def get_price_page(self):
		response = urllib.request.urlopen(self.search_url)
		#html = response.read()
		soup = BeautifulSoup(response, 'html.parser')
		mydivs = soup.findAll("div", {"class": "service-name"})
		for div in mydivs:
			a_s = div.find_all('a')
			self.price_page = a_s[0]['href']
			break
		self.price_page = "https://www.healthcarebluebook.com/page_ProcedureDetails.aspx?" + self.price_page.split("?",1)[1] 

	def get_price(self):
		response = urllib.request.urlopen(self.price_page)
		soup = BeautifulSoup(response, 'html.parser')
		mydivs = soup.findAll("div", {"class": "fee-detail-row"})
		div = mydivs[0]
		values = div.findAll("div", {"class": "value"})
		self.procedure_desc = values[0].text
		self.fair_price = values[1].text.split("$", 1)[1]
		self.low_price = soup.findAll("span", {"class": "fair"})[0].text.split("$", 1)[1]
		self.high_price = soup.findAll("span", {"class": "not-fair"})[0].text.split("$", 1)[1].split("\r", 1)[0]
		self.procedure = soup.findAll("div", {"class": "fair-price-results-summary"})[0].findAll("h3")[0].text
		#print(self.procedure + ": " + self.low_price + " " + self.high_price)

	def return_values(self):
		return {
			"procedure" : self.procedure,
			"description" : self.procedure_desc,
			"fair price" : self.fair_price,
			"low price" : self.low_price,
			"high price" : self.high_price,
		}

a = CodeSearch("73200")
print(a.return_values())