#!/usr/bin/python

from optparse import OptionParser
import os
import smtplib
from email.mime.text import MIMEText


def run_test(name, binary):
	global str_summary
	global str_output
	print "Running test: " + name + " (" + binary + ")"
	status = os.system(binary + "  --gtest_color=no > test_out.log")
	if status == 0: st = "PASSED"
	else: st = "FAILED"
	str_summary += "\n" + name + " status : " + st
	str_output += "\n\n----------" + name + "----------\n"
	fp = open("./test_out.log", "r")
	str_output += fp.read()
	fp.close()
	os.system("rm -f ./test_out.log")
	print "Done running test " + name

def mail_results():
	msg = """From: Libcds2 Team <libcds@recoded.cl>
To: %s
Subject: libcds2 test results

%s

%s""" % (options.address, str_summary, str_output)
	s = smtplib.SMTP(options.server)
	s.ehlo()
	s.sendmail("libcds@recoded.cl", options.address, msg)
	s.quit()

if __name__ == "__main__":
	str_summary = ""
	str_output = ""
	parser = OptionParser()
	parser.add_option("-a", "--mail-address", dest="address", help="Mail address to send reports")
	parser.add_option("-s", "--mail-server", dest="server", help="Mail server to send reports")
	parser.add_option("-t", "--test", dest="test", help="Test binary")
	(options, args) = parser.parse_args()
	for test in options.test.split(","):
		run_test(test, test)
	mail_results()

