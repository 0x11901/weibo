//
//  Test.swift
//  weiBo
//
//  Created by 王靖凯 on 2017/11/16.
//  Copyright © 2017年 王靖凯. All rights reserved.
//

import Foundation


class person {
    let name: String
    let age: Int
    
    init(name: String,age: Int) {
        self.name = name
        self.age = age
    }
}

class man: person {
    var sex: Bool
    
    init(name: String,age: Int,sex: Bool) {
        self.sex = sex
        super.init(name: name, age: age)
    }
}
