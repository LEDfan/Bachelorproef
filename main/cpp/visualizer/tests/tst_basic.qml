import QtQuick 2.0
import QtTest 1.1

TestCase {
    name: "MathTests"

    function test_math() {
        compare(2 + 2, 4, "2 + 2 = 4")
    }
}