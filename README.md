# Quantitative Trading Simulator and Analyzer
In this project, we aim to implement pre-existing strategies for quantitative trading and learn how various indicators and algorithms are used in the finance world.

## Authors
* Ansh Agrawal (2022CS51135)
* Aditya Prakash (2022CS11595)

## Installation
You should have the jugaad-data library installed. You can do so by running the following on any terminal.
```bash
    pip install jugaad-data
```
## Strategies Implemented
We have implemented several well known strategies like:
* **Momentum Based** Strategies like
    * Basic Strategy involving montonic price increase/decrease over a fixed period of days.
    * Moving Averages(DMA) with and without maximum hold period
    * Indicators like Moving Average Convergence/Divergence(MACD), Relative Strength Index(RSI) and Average Directional Index(ADX).
* **Linear Regression** Based strategy to train the model over past data. Since the NIFTY50 are stable stocks, linear regression fits perfectly.
* **Mean-Reverting Pairs** By keeping the tabs on *co-Integrated* stocks we can strategise on their subsequent highs and lows during the trading period.

## Usage
It is recommended to use this on a UNIX based system. Clone the repository into your local system and run any of the following commands on the terminal:
```bash
    make strategy=BASIC symbol=SBIN n=5 x=2 start_date="b" end_date="a"
    make strategy=DMA symbol=SBIN n=50 x=3 p=2 start_date="a" end_date="b"
    make strategy="DMA++" symbol=SBIN x=4 p=5 n=14 max_hold_days=28 c1=2 c2=0.2 start_date="a" end_date="b"
    make strategy=MACD symbol=SBIN x=3 start_date="a" end_date="b"
    make strategy=RSI symbol=SBIN x=3 n=14 oversold_threshold=30 overbought_threshold=70 start_date="a" end_date="b"
    make strategy=ADX symbol=SBIN x=3 n=14 adx_threshold=25 start_date="a" end_date="b"
    make strategy="LINEAR_REGRESSION" symbol=SBIN x=3 p=2 train_start_date="a" train_end_date="b" start_date="c" end_date="d"
    make strategy="BEST_OF_ALL" symbol=SBIN start_date="a" end_date="b"
    make strategy=PAIRS symbol1=SBIN symbol2=ADANIENT x=5 n=20 threshold=2 start_date="a" end_date="b"
    make strategy=PAIRS symbol1=SBIN symbol2=ADANIENT x=5 n=20 threshold=2 stop_loss_threshold=4 start_date="a" end_date="b"
```
Replace a,b,c and d by dates in DD/MM/YYYY format.
To clean existing built and data files, run:
```bash
    make clean
```
## Design Choices and Optimizations Made
We have created a modular codebase for this project, including but not limited to different directories for build and source files, abstraction between different components of the code.

We have a virtual base _strategy_ class which encapsulates the common attributes in every strategy. Each particular strategy is derived from this and has several auxiliary attributes particular to it. The historical data is extracted using a python script depending on the strategy to be implemented. 

Other optimizations include:
* We have used prefix sums to calculate rolling means and standard deviation instead of iterating through n days thus reducing the time complexity order by n. 
* For maintaining open positions, we have employed the use of deques to make all operations as _O(1)_. 
* We have used Gauss-Jordan elimination to compute inverse of a matrix instead of the traditional adjoint method. This leads to better numerical stability and better time complexity (polynomial instead of factorial)
* We have employed the use of OpenMP in C++ to _parallelize_ the running the best of all strategy.

## Insights
We played around with the values of the parameters to get a feel of how different strategies behave. We plotted all such data as follows:
<img src="# Quantitative Trading Simulator and Analyzer
In this project, we aim to implement pre-existing strategies for quantitative trading and learn how various indicators and algorithms are used in the finance world.

## Authors
* Ansh Agrawal (2022CS51135)
* Aditya Prakash (2022CS11595)

## Installation
You should have the jugaad-data library installed. You can do so by running the following on any terminal.
```bash
    pip install jugaad-data
```
## Strategies Implemented
We have implemented several well known strategies like:
* **Momentum Based** Strategies like
    * Basic Strategy involving montonic price increase/decrease over a fixed period of days.
    * Moving Averages(DMA) with and without maximum hold period
    * Indicators like Moving Average Convergence/Divergence(MACD), Relative Strength Index(RSI) and Average Directional Index(ADX).
* **Linear Regression** based strategy using past trends
* **Mean-Reverting Pairs** Trading 

## Usage
It is recommended to use this on a UNIX based system. Clone the repository into your local system and run any of the following commands on the terminal:
```bash
    make strategy=BASIC symbol=SBIN n=5 x=2 start_date="b" end_date="a"
    make strategy=DMA symbol=SBIN n=50 x=3 p=2 start_date="a" end_date="b"
    make strategy="DMA++" symbol=SBIN x=4 p=5 n=14 max_hold_days=28 c1=2 c2=0.2 start_date="a" end_date="b"
    make strategy=MACD symbol=SBIN x=3 start_date="a" end_date="b"
    make strategy=RSI symbol=SBIN x=3 n=14 oversold_threshold=30 overbought_threshold=70 start_date="a" end_date="b"
    make strategy=ADX symbol=SBIN x=3 n=14 adx_threshold=25 start_date="a" end_date="b"
    make strategy="LINEAR_REGRESSION" symbol=SBIN x=3 p=2 train_start_date="a" train_end_date="b" start_date="c" end_date="d"
    make strategy="BEST_OF_ALL" symbol=SBIN start_date="a" end_date="b"
    make strategy=PAIRS symbol1=SBIN symbol2=ADANIENT x=5 n=20 threshold=2 start_date="a" end_date="b"
    make strategy=PAIRS symbol1=SBIN symbol2=ADANIENT x=5 n=20 threshold=2 stop_loss_threshold=4 start_date="a" end_date="b"
```
To clean existing built and data files, run:
```bash
    make clean
```
## Design Choices and Optimizations Made
We have created a modular codebase for this project, including but not limited to different directories for build and source files, abstraction between different components of the code.

We have a virtual base _strategy_ class that encapsulates the common attributes in every strategy. Each particular strategy is derived from this and has several auxiliary attributes particular to it. The historical data is extracted using a Python script depending on the strategy to be implemented. 

Other optimizations include:
* We have used prefix sums to calculate rolling means and standard deviation instead of iterating through n days thus reducing the time complexity order by n. 
* For maintaining open positions, we have employed the use of deques to make all operations as _O(1)_. 
* We have used Gauss-Jordan elimination to compute the inverse of a matrix instead of the traditional adjoint method. This leads to better numerical stability and better time complexity (polynomial instead of factorial)
* We have employed the use of OpenMP in C++ to _parallelize_ the running the best of all strategy.

## Insights
We played around with the values of the parameters to get a feel of how different strategies behave. We plotted all such data as follows:
<image src="https://raw.githubusercontent.com/adityaxprakash/Benchmark-code-for-A1/main/basic.jpeg">
We notice that on increasing n, the profits increase to a max and then decrease. This makes sense as increasing n too much will highly decrease chances of a trade happening, but the trade made will almost always be profitable.

<img src="https://raw.githubusercontent.com/adityaxprakash/Benchmark-code-for-A1/main/dma.jpeg">
<img src="https://raw.githubusercontent.com/adityaxprakash/Benchmark-code-for-A1/main/dma_n.jpeg">
We notice increasing p increases the profit gained. On the other hand, as n increases, the general trend is that the profit falls ignoring some abrupt changes.

<image src="https://raw.githubusercontent.com/adityaxprakash/Benchmark-code-for-A1/main/macd.jpeg">
On increasing p, we notice an initial rise in profits following a steady decrease.
<img src="https://raw.githubusercontent.com/adityaxprakash/Benchmark-code-for-A1/main/RSI.jpeg">
Here too on increasing n, we see a decrease in profits.

<img src="https://raw.githubusercontent.com/adityaxprakash/Benchmark-code-for-A1/main/dma%2B%2B.jpeg">
 Increasing n on DMA++ we see the profits steadily fall before baselining.

 The common reason for all the trends can be explained as follows: the values of n and p have an optimum range. Below that range, we are not selective enough and above it we are so restrictive that we deny most of the trades. This leads to a loss in the long run. This was the common trend we found among all strategies i.e. the parameters have some fixed ranges where the profits are the maximum and going beyond that range is unprofitable.

## Other Insights
* Every now and then a stock splits into other stocks. Hence price and volume are not good indicators in linear regression. We should use the multiplication i.e. value*volume as a parameter.
* Jugaad-data does not give singular values for a stock on a day. Hence we have to remove duplicates while loading data.
## Mean Reverting Pairs Strategy using Stop-Loss
We have incorporated stop-loss into our mean reverting pairs strategy to minimize losses if the stock behaves unexpectedly. 

Suppose we have shorted the spread. When the z-score crosses the positive stop-threshold, buying back prevents our losses from piling up. On the other hand, the stock crossing the negative stop-threshold is also an indicator to close the position. The reason behind this choice is that once the score crosses the stop-threshold on either side, its an indication of unpredictability. If the z-score drops heavily, we have already made a good bunch of profit and its beneficial for us to close this highly profitable opening before it inevitably recovers.

To give a general overview of the entire strategy, consider we have an overall short position currently. 

* If we get a buy signal next, we try to clear the last shorted stock as well as all stocks which cross the stop-threshold on either side. Even if these coincide, we remove only these stocks.
* If we get a sell signal next, we consider the net change in position after shorting the current stock and clearing the positions which cross the z-score. Note: the net position after this will always lie in the position constraints.
* If we do not either signals, we stil clear all stocks which exceed the stop-loss-threshold on either side and do not create any new openings.

A long position can be handled similarly.
