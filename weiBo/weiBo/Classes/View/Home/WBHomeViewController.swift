//
//  WBHomeViewController.swift
//  weiBo
//
//  Created by 王靖凯 on 2016/11/24.
//  Copyright © 2016年 王靖凯. All rights reserved.
//

import UIKit

class WBHomeViewController: WBBaseViewController {
    lazy var dataSource: [WBStatusViewModel] = []
    let cellReuseIdentifier = "dasfsdfsfsf"
    
    override func viewDidLoad() {
        super.viewDidLoad()
        loadStatus()
        // Do any additional setup after loading the view.
    }

}

// MARK: - 读取数据
extension WBHomeViewController {
    fileprivate func loadStatus() {
        NetworkTool.shared.requestForHomeStatus(success: { (response) in
            guard let dictionary = response as? [String : Any],let json = dictionary["statuses"] else{
                print("获取json错误")
                return
            }
            
            guard let status = NSArray.yy_modelArray(with: WBStatusModel.self, json: json) else{
                print("json转模型错误")
                return
            }
            
            guard let statues = status as? [WBStatusModel] else{
                print("statues类型错误")
                return
            }
            
            var arrM: [WBStatusViewModel] = []
            for obj in statues {
                arrM.append(WBStatusViewModel(status: obj))
            }
            
            self.dataSource += arrM
            self.tableView.reloadData()
        }, failure: {
            (err: Error) in
            print(err)
        })
    }
}

// MARK: - reloadData
extension WBHomeViewController {
    
    override func setupTableView() {
        super.setupTableView()
        tableView.register(WBStatusCell.self, forCellReuseIdentifier: cellReuseIdentifier)
        tableView.estimatedRowHeight = 200
        tableView.rowHeight = UITableViewAutomaticDimension
    }
    
    override func tableView(_ tableView: UITableView, numberOfRowsInSection section: Int) -> Int {
        return dataSource.count
    }
    
    override func tableView(_ tableView: UITableView, cellForRowAt indexPath: IndexPath) -> UITableViewCell {
        let cell = tableView.dequeueReusableCell(withIdentifier: cellReuseIdentifier, for: indexPath) as! WBStatusCell
        cell.status = dataSource[indexPath.row]
        return cell
    }
}
