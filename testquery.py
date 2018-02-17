from bs4 import BeautifulSoup
import urllib.request
from pymongo import MongoClient

class CodeSearch:
	def __init__(self, code):
		self.code = code
		self.legit = True
		self.search_url = "https://www.healthcarebluebook.com/page_SearchResults.aspx?SearchTerms=" + code + "&tab=ShopForCare"
		self.get_price_page()
		if self.legit:
			self.get_price()

	def get_price_page(self):
		response = urllib.request.urlopen(self.search_url)
		soup = BeautifulSoup(response, 'html.parser')
		legit = soup.findAll("h3", {"class": "physician"})
		print(legit)
		if len(legit) == 0:
			self.legit = False
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
			"code" : self.code,
			"procedure" : self.procedure,
			"description" : self.procedure_desc,
			"fair_price" : self.fair_price,
			"low_price" : self.low_price,
			"high_price" : self.high_price,
		}

#codes = [70480, 70481, 70482, 70486, 70487, 70488, 70490, 70491, 70492, 73700, 73701, 73702, 70450, 70460, 70470, 72125, 72126, 72127, 71250, 71260, 71270, 72128, 72129, 72130, 74176, 74177, 74178, 72131, 72132, 72133, 70540, 70543, 70336, 73221, 73222, 73223, 73218, 73220, 73721, 73722, 73723, 73718, 73720, 70551, 70553, 72141, 72156, 71550, 71552, 77059, 72146, 72157, 74181, 74183, 72148, 72158, 72195, 72197]
for code in codes:
	strcode = str(code)
	a = CodeSearch(strcode)
	if a.legit:
		client = MongoClient('mongodb://foo:bar@ds125198.mlab.com:25198/codelookup')
		db = client.codelookup
		collection = db.procedures
		post = a.return_values()
		post_id = collection.insert_one(post).inserted_id
