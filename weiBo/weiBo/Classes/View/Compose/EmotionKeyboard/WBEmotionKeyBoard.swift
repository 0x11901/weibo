//
//  WBEmotionKeyBoard.swift
//  weiBo
//
//  Created by 王靖凯 on 2016/12/5.
//  Copyright © 2016年 王靖凯. All rights reserved.
//

import UIKit

private let itemReuseIdentifier = "asfasdfsgdfghd"

class WBEmotionKeyBoardLayout: UICollectionViewFlowLayout {
    
    override func prepare() {
        minimumLineSpacing = 0
        minimumInteritemSpacing = 0
        scrollDirection = .horizontal
        
        itemSize = CGSize(width: screenWidth, height: 216 - 37)
    }
    
}

class WBEmotionKeyBoard: UIView {
    
    var dataSource = WBEmotionTool.shared.emotionsDataSource()
    
    fileprivate lazy var emojiCollectionView: UICollectionView = {
        let emoji = UICollectionView(frame:
            .zero, collectionViewLayout: WBEmotionKeyBoardLayout())
        emoji.delegate = self
        emoji.dataSource = self
        emoji.register(WBEmotionKeyboardCell.self, forCellWithReuseIdentifier: itemReuseIdentifier)
        emoji.showsHorizontalScrollIndicator = false
        emoji.showsVerticalScrollIndicator = false
        emoji.isPagingEnabled = true
        emoji.backgroundColor = UIColor.white
        return emoji
    }()
    
    lazy var toolBar: WBEmotionToolBar = WBEmotionToolBar()
    
    fileprivate lazy var pageControl: UIPageControl = {
        let page = UIPageControl()
        page.numberOfPages = self.dataSource[0].count
        page.currentPage = 0
        page.setValue(UIImage(named:"compose_keyboard_dot_selected"), forKey: "_currentPageImage")
        page.setValue(UIImage(named:"compose_keyboard_dot"), forKey: "_pageImage")
        page.isUserInteractionEnabled = false
        return page
    }()
    
    override init(frame: CGRect) {
        super.init(frame: frame)
        setupView()
    }
    
    required init(coder: NSCoder) {
        fatalError("init(coder:) has not been implemented")
    }
    
}

extension WBEmotionKeyBoard {
    
    fileprivate func setupView() {
        addSubview(toolBar)
        toolBar.delegate = self
        toolBar.snp.makeConstraints { (make) in
            make.leading.trailing.bottom.equalTo(self)
            make.height.equalTo(37)
        }
        
        addSubview(emojiCollectionView)
        emojiCollectionView.snp.makeConstraints { (make) in
            make.leading.trailing.top.equalTo(self)
            make.bottom.equalTo(toolBar.snp.top)
        }
        
        emojiCollectionView.addSubview(pageControl)
        pageControl.snp.makeConstraints { (make) in
            make.left.equalTo(self)
            make.right.equalTo(self)
            make.top.equalTo(emojiCollectionView.snp.bottom)
            make.bottom.equalTo(toolBar.snp.top)
        }
    }
    
    func changePageContol (indexPath: IndexPath) {
        //pageControl要改变? 1. numberOfPages, 2.currentPage
        //numberOfPages: 当前的组的cell的个数, 从数据源来的
        //currentPage: 0
        pageControl.numberOfPages = dataSource[indexPath.section].count
        pageControl.currentPage = indexPath.item
    }
    
}

extension WBEmotionKeyBoard: UICollectionViewDataSource {
    
    public func numberOfSections(in collectionView: UICollectionView) -> Int {
        return dataSource.count
    }
    
    public func collectionView(_ collectionView: UICollectionView, numberOfItemsInSection section: Int) -> Int{
        return dataSource[section].count
    }
    
    public func collectionView(_ collectionView: UICollectionView, cellForItemAt indexPath: IndexPath) -> UICollectionViewCell{
        let cell = collectionView.dequeueReusableCell(withReuseIdentifier: itemReuseIdentifier, for: indexPath)  as! WBEmotionKeyboardCell
        cell.model = dataSource[indexPath.section][indexPath.item]
        return cell
    }
    
}

extension WBEmotionKeyBoard: UICollectionViewDelegate {
    
    func scrollViewDidScroll(_ scrollView: UIScrollView) {
        if emojiCollectionView.visibleCells.count > 1 {
            let offset = scrollView.contentOffset.x
            let leftOriginX = emojiCollectionView.visibleCells[0].frame.origin.x
            let rightOriginX = emojiCollectionView.visibleCells[1].frame.origin.x
            if abs(leftOriginX - offset) < abs(rightOriginX - offset) {
                let index = (emojiCollectionView.indexPath(for: emojiCollectionView.visibleCells[0]))!
                toolBar.index = index.section
                changePageContol(indexPath: index)
            }else{
                let index = (emojiCollectionView.indexPath(for: emojiCollectionView.visibleCells[1]))!
                toolBar.index = index.section
                changePageContol(indexPath: index)
            }
        }
    }
    
}

extension WBEmotionKeyBoard: WBEmotionToolBarDelegate {
    
    func emotionToolBar(_ emotionToolBar: WBEmotionToolBar, didSelectedAtIndex index: Int) {
        let indexPath = IndexPath(item: 0, section: index)
        emojiCollectionView.scrollToItem(at: indexPath, at: UICollectionViewScrollPosition.left, animated: false)
        changePageContol(indexPath: indexPath)
    }
    
}













