import time
import MySQLdb as mdb

WRN = '\033[93m[WRN]\t\033[0m'
DBG = '\033[94m[DBG]\t\033[0m'
INF = '\033[92m[INF]\t\033[0m'
ERR = '\033[91m[ERR]\t\033[0m'

initScript = [
"DROP DATABASE IF EXISTS clusterTest;",
"CREATE DATABASE clusterTest; USE clusterTest;",
"CREATE TABLE tableTest (id INT NOT NULL) engine=ndb;",
"INSERT INTO tableTest VALUES (1),(2),(3),(4);"
]

numOfTestTransactions = 1000

def connectToDatabase():
	for actualTry in range(1,10):
		print(INF+"Trying connect to DB %d of 10..." % actualTry)
		try:
			connection = mdb.connect(host='127.0.0.1',port=3307, user='root', passwd='root')
			return connection
		except mdb.Error, e:
			print(WRN+"Couldn't connecto to database. Will retry in 5 seconds...")
			time.sleep(15)

	print (ERR+"Couldn't connect to database")
	return None

def runScript(script, connection):
	for statement in script:
		try:
			cursor = connection.cursor()
			cursor.execute(statement)
			cursor.close()	
		except mdb.MySQLError:
			print(ERR+"Error processing MySQL query")


def validatePreviousTransacton(element):
	connection = connectToDatabase();
	if (not connection):
		exit(1);
	cursor = connection.cursor()
	cursor.execute("USE clusterTest;")
	cursor.execute("SELECT * FROM tableTest WHERE id=%s;" % str(element))
	result = cursor.fetchall()
	if (len(result) > 0):
		print(INF+"Database has passed integrity check. ")
	else:
		print(ERR+"Database has failed integrity check. ")

def main():
	connection = connectToDatabase();
	if (not connection):
		exit(1);

	runScript(initScript, connection)

	print(INF+"Database initiated. Will perform write tests now...")

	start = time.time()
	cursor = connection.cursor()
	for element in range(0,numOfTestTransactions):
		try:
			cursor.execute("INSERT INTO tableTest VALUES (%s);" % str(element))
			connection.commit()

		except mdb.Error, e:
			print(ERR+str(e) + " MySQL has received internal error. Validating previous transacton...")
			validatePreviousTransacton(element-1);
			break
	execTime = time.time()-start
	print(INF+'Execution time: %s [s]. Performance: %s [requests/s]' % (execTime, numOfTestTransactions/execTime))

if __name__ == '__main__':
	main()
