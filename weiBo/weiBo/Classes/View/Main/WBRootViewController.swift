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
        setupTabBar()
    }
    
    func setupTabBar() {
        if let url = Bundle.main.url(forResource: "main", withExtension: "json"){
            if let json = try? Data(contentsOf: url){
                if let array = try? JSONSerialization.jsonObject(with: json, options: []) as! [[String : Any]] {
                    for dict in array{
                        if let clsName = dict["clsName"] as? String{
                            let title = dict["title"] as? String
                            let image = dict["imageName"] as? String
                            addChildViewController(childController: clsName, title: title, image: image)
                        }
                    }
                }
            }
        }
        
    }
    
    func addChildViewController(childController: String, title: String?, image: String?){
        if let vcClass = NSClassFromString(Bundle.main.infoDictionary?["CFBundleName"] as! String + "." + childController) as? UIViewController.Type{
            let controller = vcClass.init()
           
            if let title = title {
                controller.title = title
            }
            if var image = image {
                image = "tabbar_\(image)"
                controller.tabBarItem.image = UIImage(named: image)?.withRenderingMode(.alwaysOriginal)
                controller.tabBarItem.selectedImage = UIImage(named: image + "_selected")?.withRenderingMode(.alwaysOriginal)
            }
            addChildViewController(UINavigationController(rootViewController: controller))
        }
    }
    
    func add() {
        <#function body#>
    }
}
