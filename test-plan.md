# Method
We use travis CI and jenkins to run our test automatically after every commit. If a commit then causes a build to fail we will be notified by email or Telegram (Instant Messaging).

# Unit tests

Using the Google Test and Google Mock frameworks the individual units of the program will be tested. 
The difference with scenario tests is that only one unit is tested at a time.

## Geo Generator

Given that at the time of writing the design for gen-geo isn't entirely finished, the test plan should not be considered completely accurate.
The different readers used can be easily tested, they get some input (i.e. an input stream to read/parse) and will output the data using some datastructure. 
By using white-box testing the internal structure of the code can be studied, allowing us to more easily identify edge cases (e.g. character escaping in the input format, missing keys ...).
The same holds for the Geo

### GeoGrid Config
### Cities CSV Reader
### Commutes CSV Reader
### GeoGrid JSON Writer 

## Pop Generator
### Households CSV Reader
### GeoGrid JSON Reader

# Scenario tests
### InfluenceR0Runs.cpp
Functionality tested: This tests the spread of a disease depending on the R0 factor that is given. 
Implementation: It is implemented for some known scenario's. It checks if the result of the test is withing reasonable bounds of the expected value. As it is not completely deterministic but uses pseudo random numbers.

