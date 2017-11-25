//
//  WBHomeViewController.swift
//  weiBo
//
//  Created by 王靖凯 on 2016/11/24.
//  Copyright © 2016年 王靖凯. All rights reserved.
//

import UIKit
import SVProgressHUD
import GTMWebKit

class WBHomeViewController: WBBaseViewController {
    var items: [DataItem] = []
    lazy var listViewModel = WBStatusListViewModel()
    let cellReuseIdentifier = "dasfsdfsfsf"
    
    override func viewDidLoad() {
        super.viewDidLoad()
        loadStatus(isPull: true)
        // Do any additional setup after loading the view.
        NotificationCenter.default.addObserver(self, selector: #selector(didSelectedAnImage(sender:)), name: clickThumbImage, object: nil)
        NotificationCenter.default.addObserver(self, selector: #selector(didSelectedAHyperlink(sender:)), name: clickHyperlink, object: nil)
    }
    
    deinit {
        NotificationCenter.default.removeObserver(self)
    }
    
    override func setupTableView() {
        super.setupTableView()
        tableView.register(WBStatusCell.self, forCellReuseIdentifier: cellReuseIdentifier)
        tableView.estimatedRowHeight = 200
        tableView.rowHeight = UITableViewAutomaticDimension
    }
    
}

// MARK: - 读取数据
extension WBHomeViewController {
    fileprivate func loadStatus(isPull: Bool) {
        listViewModel.loadDate(isPull: isPull) { (isSuccess) in
            if isSuccess {
                if isPull {
                    self.header.endRefreshing()
                }else{
                    self.footer.endRefreshing()
                }
                self.tableView.reloadData()
            }else{
                if isPull {
                    self.header.endRefreshing()
                }else{
                    self.footer.endRefreshing()
                }
                SVProgressHUD.showError(withStatus: "加载失败")
            }
        }
    }
}

// MARK: - reloadData
extension WBHomeViewController {
    
    override func tableView(_ tableView: UITableView, numberOfRowsInSection section: Int) -> Int {
        return listViewModel.dataSource.count
    }
    
    override func tableView(_ tableView: UITableView, cellForRowAt indexPath: IndexPath) -> UITableViewCell {
        let cell = tableView.dequeueReusableCell(withIdentifier: cellReuseIdentifier, for: indexPath) as! WBStatusCell
        cell.status = listViewModel.dataSource[indexPath.row]
        return cell
    }
    
    override func headerRefresh() {
        loadStatus(isPull: true)
    }
    
    override func footerRefresh() {
        loadStatus(isPull: false)
    }
    
    override func whenLoginIsSucess() {
        super.whenLoginIsSucess()
        self.loadStatus(isPull: true)
    }
}

import ImageViewer
extension WBHomeViewController: GalleryItemsDataSource {
    func itemCount() -> Int {
        
        return items.count
    }
    
    func provideGalleryItem(_ index: Int) -> GalleryItem {
        
        return items[index].galleryItem
    }
    
    struct DataItem {
        let imageView: UIImageView
        let galleryItem: GalleryItem
    }
    
    @objc fileprivate func didSelectedAnImage(sender: Notification) {
        
        if let index = sender.userInfo?[indexKey] as? NSNumber,let urls = sender.userInfo?[urlsKey] as? [String] {
            
            let browser = WBPhotoBrowserViewController(index: index.intValue, images: urls)
            self.presentImageGallery(browser.galleryViewController)
        }
        
        
    }
    
    @objc private func didSelectedAHyperlink(sender: Notification) {
        if let hyperlink = sender.userInfo?[hyperlinkTextKey] as? String {
            let reg = "http(s)?:\\/\\/([\\w-]+\\.)+[\\w-]+(\\/[\\w- .\\/?%&=]*)?"
            let pre = NSPredicate(format: "SELF MATCHES %@", reg)
            if pre.evaluate(with: hyperlink) {
                let webVC = GTMWebViewController(with: hyperlink, navigType: .navbar)
                webVC.hidesBottomBarWhenPushed = true
                self.navigationController?.pushViewController(webVC, animated: true)
            }else{
                console.debug(hyperlink)
            }
        }
    }
    
}