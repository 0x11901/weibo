//
//  WBVisitorModel.swift
//  weiBo
//
//  Created by 王靖凯 on 2016/11/26.
//  Copyright © 2016年 王靖凯. All rights reserved.
//

import UIKit

class WBVisitorModel: NSObject {
    var imageName: String? = nil
    var message: String? = nil
    var isAnima: Bool = false
    
    init(dictionary: [String : Any]) {
        super.init()
        setValuesForKeys(dictionary)
    }
    
    override func setValue(_ value: Any?, forUndefinedKey key: String) {
        print(key)
    }
}
