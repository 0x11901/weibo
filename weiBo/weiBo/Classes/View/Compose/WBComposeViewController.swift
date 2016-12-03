//
//  WBComposeViewController.swift
//  weiBo
//
//  Created by 王靖凯 on 2016/11/24.
//  Copyright © 2016年 王靖凯. All rights reserved.
//

import UIKit

class WBComposeViewController: UIViewController {

    override func viewDidLoad() {
        super.viewDidLoad()

        setupUI()
    }

}

extension WBComposeViewController {
    
    fileprivate func setupUI() {
        _ = UIButton(title: "取消", fontSize: 14, color: .white, bgImage: "", target: self, action: #selector(cancell))
    }
    
}

extension WBComposeViewController {
    
    @objc fileprivate func cancell() {
        
    }
    
}
