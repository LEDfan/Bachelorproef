import QtQuick 2.0
import QtTest 1.1
import "components"

TestCase {
    name: "MathTests"

    ContactCenterViewer {}

    function test_math() {
        compare(2 + 2, 4, "2 + 2 = 4")
    }
}
