delimiter $$
USE forex
DROP TABLE IF EXISTS CURRENCY_PAIR;

CREATE TABLE CURRENCY_PAIR (
  `date` char(10) NOT NULL,
  `hour` int(11) NOT NULL,
  `minute` int(11) NOT NULL,
  `second` int(11) NOT NULL,
  `sequence` int(11) NOT NULL,
  `bid` float NOT NULL,
  `ask` float NOT NULL,
  PRIMARY KEY (`date`,`hour`,`minute`,`second`,`sequence`),
  KEY `index_CURRENCY_PAIR_date_hour` (`date`,`hour`)
);

DROP TABLE IF EXISTS CURRENCY_PAIR_CANDLES;
CREATE TABLE CURRENCY_PAIR_CANDLES (
  `date` char(10) NOT NULL,
  `hour` int(11) NOT NULL,
  `open` float NOT NULL,
  `high` float NOT NULL,
  `low` float NOT NULL,
  `close` float NOT NULL,
  PRIMARY KEY (`date`,`hour`)
);

LOAD DATA LOCAL INFILE '/home/john/projects/forex/data-cleaning/CURRENCY_PAIR_TRANSFORMED.csv'
INTO TABLE CURRENCY_PAIR
FIELDS TERMINATED BY "," OPTIONALLY ENCLOSED BY """";

Insert Into CURRENCY_PAIR_CANDLES (date,hour,open,high,low,close)
select date, hour, 1, max(bid), min(bid),1 from CURRENCY_PAIR Group by date,hour;


-- update open price
update CURRENCY_PAIR_CANDLES SET open = (select bid from CURRENCY_PAIR where date = CURRENCY_PAIR_CANDLES.date and hour = CURRENCY_PAIR_CANDLES.hour LIMIT 1);

-- update close price
update CURRENCY_PAIR_CANDLES SET close = (select bid from CURRENCY_PAIR where date = CURRENCY_PAIR_CANDLES.date and hour = CURRENCY_PAIR_CANDLES.hour Order By minute desc, second DESC, sequence desc  LIMIT 1);

