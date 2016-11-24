//
//  WBRootViewController.swift
//  weiBo
//
//  Created by 王靖凯 on 2016/11/24.
//  Copyright © 2016年 王靖凯. All rights reserved.
//

import UIKit

class WBRootViewController: UITabBarController {
    override func viewDidLoad() {
        setupUI()
    }
}

// MARK: - UI相关
extension WBRootViewController {
    
    func setupUI() {
        view.backgroundColor = UIColor.randomColor()
        if let url = Bundle.main.url(forResource: "main", withExtension: "json"){
            if let json = try? Data(contentsOf: url){
                if let array = try? JSONSerialization.jsonObject(with: json, options: []) as! [[String : Any]] {
                    print(array)
                }
            }
        }
    }
    
}
